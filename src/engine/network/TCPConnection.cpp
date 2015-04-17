#include "TCPConnection.h"


TCPConnection::TCPConnection() : Socket() {
    this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    fill(sendBuffer.begin(), sendBuffer.end(), 0);
    fill(receiveBuffer.begin(), receiveBuffer.end(), 0);
}


TCPConnection::TCPConnection(int sockFd, SocketAddress remAddr) : Socket() {
    this->sock = sockFd;
    this->remoteAddress = remAddr;
    this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    fill(sendBuffer.begin(), sendBuffer.end(), 0);
    fill(receiveBuffer.begin(), receiveBuffer.end(), 0);
}


TCPConnection::~TCPConnection() {
    this->Close();
}

SocketError TCPConnection::Connect(const string & ip, const string & port) {
    AddressInfo *iter = this->DNSLookup(ip, port, SOCK_STREAM);

    for (; iter; iter = iter->ai_next) {
        sock = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (sock == INACTIVE_SOCKET) {
            continue;
        }
        if (::connect(sock, iter->ai_addr, iter->ai_addrlen) != 0) {
            this->Close(sock);
            continue;
        }
        break;
    }
    if (iter != nullptr) {
        freeaddrinfo(iter);
    }
    return (sock == INACTIVE_SOCKET) ? SE_NOCONNECT : SE_NOERR;
}


SocketError TCPConnection::Send(const Packet & packet) {
    // if theres nothing to send do nothing
    if (packet.size() == 0) {
        return SE_NOERR;
    }

    // if there's a problem with the packet's size return the PACKETSIZE error
    if (packet.size() > MAX_PACKET_SIZE || packet.size() < 0) {
        return SE_PACKETSIZE;
    }
  
    // reserve enough space in the buffer to store the entire packet.
    if (packet.size() > sendBuffer.capacity() - sendBuffer.size()) {
        sendBuffer.reserve(sendBuffer.size() + packet.size() + sizeof(packet.size()));
    }

    // write the packet size to the buffer
    uint32_t mask = 0xFF000000;
    uint32_t size = HostToNet(static_cast<uint32_t>(packet.size()));
    for (int i = 3; i >= 0; --i) {
        sendBuffer.push_back((size & mask) >> (i * 8));
        mask >>= 8;
    }

    // put the body of the packet in the buffer
    for (auto it = packet.begin(); it != packet.end(); ++it) {
        sendBuffer.push_back(*it);
    }

    for (auto it = sendBuffer.begin(); it != sendBuffer.end(); ++it) {
        std::cout << std::to_string(*it) << " ";
    }
    std::cout << "\n";
    // send as much as the OS will send
    int numSent = this->Send(sendBuffer.data(), sendBuffer.size());

    if (numSent == 0) {
        return SE_DISCONNECTED;
    } else if (numSent < 0) {     // if less than 0 there was some error so return it
        return this->GetError();
    }
    
    // buffer positions
    unsigned int vpos;
    unsigned int spos;

    // move all the unsent (leftover) bytes to the front of the buffer
    for (vpos = numSent, spos = 0; vpos < sendBuffer.size(); ++spos, ++vpos) {
        sendBuffer[spos] = sendBuffer[vpos];
    }
    // change the size to be correct after the send
    sendBuffer.resize(sendBuffer.size() - numSent);
    return SE_NOERR;
}



SocketError TCPConnection::Receive(Packet & packet) {

    std::cout << "1" << std::endl;
    // Try to grab a packet from the buffer before calling receive.
    if (this->FillFromBuffer(packet)) {
        return SE_NOERR;
    }

    // if the buffer is full and we havent gotten a packet the packet's too large.
    if (receiveBuffer.size() > MAX_PACKET_SIZE) {
        return SE_PACKETSIZE;
    }

    std::cout << "2" << std::endl;
    // Caclulate how much buffer space we have. If the buffer is small try to make it bigger
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();
    if (bytesAvail < FREE_THRESHOLD && receiveBuffer.capacity() < MAX_SOCKET_BUFSIZ) {
        this->ExpandReceiveBuff();
    }

    uint32_t buffPosition = receiveBuffer.size();
    receiveBuffer.resize(receiveBuffer.capacity());
    std::cout << "3  bytes avail  " << bytesAvail <<  " receivebuff.size : " << receiveBuffer.size() << std::endl;
    // receive as much as we can
    int bytesRecvd = this->Receive(receiveBuffer.data() + buffPosition, bytesAvail);
    std::cout << "bytesRecvd " << bytesRecvd << std::endl;

    // check for errors
    if (bytesRecvd == 0) {          // 0 means the socket isnt connected anymore
        return SE_DISCONNECTED;
    } else if (bytesRecvd < 0) {    // < 0 means there was some error
        return this->GetError();
    }
    std::cout << "4 receivebuffer.size" << receiveBuffer.size() <<  "  prop new size " << receiveBuffer.size() + bytesRecvd << std::endl;
    // update the buffer size to reflect the number of bytes we got from the network
    receiveBuffer.resize(buffPosition + bytesRecvd);
    for (auto it = receiveBuffer.begin(); it != receiveBuffer.end(); ++it) {
        std::cout << std::to_string(*it) << " ";
    }
    std::cout << "\n";

    this->FillFromBuffer(packet);               // try to get a packet from the buffer
    return SE_NOERR;
}


int TCPConnection::Send(const void* data, int size) {
#ifdef _WIN32
    return ::send(sock, static_cast<const char*>(data), size, 0);
#else
    return ::send(sock, data, size, 0);
#endif
}


int TCPConnection::Receive(void* buffer, int buffSize) {
#ifdef _WIN32
    return ::recv(sock, static_cast<char*>(buffer), buffSize, 0);
#else
    return ::recv(sock, buffer, buffSize, 0);
#endif
}


bool TCPConnection::FillFromBuffer(Packet & packet) {
    // if the buffer is as long as the header read the header so we know how many bytes to grab
    if (receiveBuffer.size() < BYTES_IN_HEADER) {
        std::cout << "not enough data to fill. returning..." << std::endl;
        return false;
    }

    // If the buffer doesn't contain a complete packet then return false
    if (receiveBuffer.size() - BYTES_IN_HEADER < (unsigned int) nextPacketSize) {
        std::cout << "returning false" << std::endl;
        return false;
    }
    uint32_t x = 0;
    x |= receiveBuffer[0] << 24; 
    x |= receiveBuffer[1] << 16;
    x |= receiveBuffer[2] << 8;
    x |= receiveBuffer[3];
    std::cout << " x :::: " << x << std::endl;
    nextPacketSize = NetToHost(x);
    std::cout << "next packet size ::::::: " << nextPacketSize << std::endl;
    std::cout << "copying..." << std::endl;
    // Copy a packet into the packet buffer we were passed
    for (int i = 0; i < nextPacketSize; ++i) {
        std::cout << "val : " << receiveBuffer[i + BYTES_IN_HEADER] << std::endl;
        packet.push_back(receiveBuffer[i + BYTES_IN_HEADER]);
    }
    std::cout << "done." << std::endl;
    // calculate the leftover bytes then move them to the front of the buffer. THis way the nextPacketSize is always the 
    // first thing in the buffer. Resize the buffer so the size member variable is accurate.
    int bytesRemaining = receiveBuffer.size() - nextPacketSize - BYTES_IN_HEADER;
    std::cout << "bytes remaining " << bytesRemaining << std::endl;
    //memmove((void*) &receiveBuffer, (void*) (&receiveBuffer[0] + BYTES_IN_HEADER + nextPacketSize), bytesRemaining);
    int i = 0;

    for (; i < bytesRemaining; ++i) {
        receiveBuffer[i] = receiveBuffer[i + BYTES_IN_HEADER + nextPacketSize];
        std::cout << receiveBuffer[i] << std::endl;
    }
    receiveBuffer.resize(bytesRemaining);

    return true;
}


void TCPConnection::ExpandReceiveBuff() {
    int newSize = DEFAULT_SOCKET_BUFSIZ;
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();

    while (bytesAvail < FREE_THRESHOLD && receiveBuffer.capacity() < MAX_SOCKET_BUFSIZ) {
        newSize <<= 1;
        bytesAvail = newSize - receiveBuffer.size();
    }

    receiveBuffer.reserve(newSize);
}
