#include "TCPConnection.h"


TCPConnection::TCPConnection() {
    this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
}


TCPConnection::TCPConnection(int sockFd, SocketAddress remAddr) {
    this->sock = sockFd;
    this->remoteAddress = remAddr;
	this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
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
    if(packet.size() == 0) {
        return SE_NOERR;
    }

    // if there's a problem with the packet's size return the PACKETSIZE error
    if (packet.size() > MAX_PACKET_SIZE || packet.size() < 0) {
        return SE_PACKETSIZE;
    }

    // reserve enough space in the buffer to store the entire packet.
    if (packet.size() > sendBuffer.capacity() - sendBuffer.size()) {
        sendBuffer.resize(sendBuffer.size() + packet.size() + sizeof(packet.size()));
    }

    // get the packet size so we can send that as the packet's header
    uint32_t size = static_cast<uint32_t>(packet.size());
    sendBuffer.push_back(HostToNet(size)); 

    // put the body of the packet in the buffer
    for (auto it = packet.begin(); it != packet.end(); ++it) {
        sendBuffer.push_back(*it);
    }

    // send as much as the OS will send
    int numSent = this->Send((void*) &sendBuffer, sendBuffer.size());

    // if we get 0 then there is a connection problem 
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
    sendBuffer.resize(packet.size() - numSent);
    return SE_NOERR;
}



SocketError TCPConnection::Receive(Packet & packet) {
    // if the buffer is full and we havent gotten a packet the packet's too large.
    if (receiveBuffer.size() > MAX_PACKET_SIZE + BYTES_IN_HEADER) {
        return SE_PACKETSIZE;
    }
    
    // Try to grab a packet from the buffer before calling receive.
    if (this->FillFromBuffer(packet)) {    
        return SE_NOERR;
    }

    // Caclulate how much buffer space we have. If the buffer is small try to make it bigger
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();
    if(bytesAvail < FREE_THRESHOLD && receiveBuffer.size() < MAX_SOCKET_BUFSIZ) {
        this->ExpandReceiveBuff();
    }

    // receive as much as we can
    int bytesRecvd = this->Receive((void*) &receiveBuffer[receiveBuffer.size()], bytesAvail);

    // check for errors
    if (bytesRecvd == 0) {          // 0 means the socket isnt connected anymore
        return SE_DISCONNECTED;
    } else if (bytesRecvd < 0) {    // < 0 means there was some error
        return this->GetError();
    }
    
    // update the buffer size to reflect the number of bytes we got from the network
    receiveBuffer.resize(receiveBuffer.size() + bytesRecvd);
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
    if (receiveBuffer.size() >= BYTES_IN_HEADER) {
        nextPacketSize = NetToHost(receiveBuffer[0] | receiveBuffer[1] | receiveBuffer[2] | receiveBuffer[3]);
    }

    // If the buffer doesn't contain a complete packet then return false
    if (receiveBuffer.size() - BYTES_IN_HEADER < (unsigned int) nextPacketSize) {
        return false;
    }

    // Copy a packet into the packet buffer we were passed
    for (int i = 0; i < nextPacketSize; ++i) {
        packet[i] = receiveBuffer[i + BYTES_IN_HEADER];
    }

    // calculate the leftover bytes then move them to the front of the buffer. THis way the nextPacketSize is always the 
    // first thing in the buffer. Resize the buffer so the size member variable is accurate.
    int bytesRemaining = receiveBuffer.size() - nextPacketSize - BYTES_IN_HEADER;
    memmove((void*) &receiveBuffer, (void*) (&receiveBuffer + BYTES_IN_HEADER + nextPacketSize), bytesRemaining);
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
