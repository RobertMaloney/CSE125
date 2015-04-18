#include "TCPConnection.h"


TCPConnection::TCPConnection() : Socket() {
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    sendBuffer.resize(0);
    receiveBuffer.resize(0);
}


TCPConnection::TCPConnection(int sockFd, SocketAddress remAddr) : Socket() {
    this->sock = sockFd;
    this->remoteAddress = remAddr;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    sendBuffer.resize(0);
    receiveBuffer.resize(0);
}


TCPConnection::~TCPConnection() {
    this->Close();
}


SocketError TCPConnection::Connect(const string & ip, const string & port) {
    AddressInfo *iter = this->DNSLookup(ip, port, SOCK_STREAM);
    // iterate over available address and try to connect. if ones successful we break
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
    // make sure to release the linked list
    if (iter != nullptr) {
        memmove(static_cast<void*>(&remoteAddress), static_cast<const void*>(iter->ai_addr), sizeof(*(iter->ai_addr)));
        freeaddrinfo(iter);
    }
    // if we went through everything and the socket isn't valid we couldnt connect
    return (sock == INACTIVE_SOCKET) ? SE_NOCONNECT : SE_NOERR;
}


SocketError TCPConnection::Send(const Packet & packet) {
    this->WriteToBuffer(packet);    // fill the buffer
    return this->Send();            // send everything we can
}


SocketError TCPConnection::Send(const deque<Packet> & packets) {
    // put them all in the buffer then send as much as we can
    for (auto it = packets.begin(); it != packets.end(); ++it) {
        this->WriteToBuffer(*it);
    }
    return this->Send();
}


SocketError TCPConnection::Receive(Packet & packet) {
    unsigned int pos = 0;
    packet.clear();
    // try to grab a packet from the buffer. if successful shift the buffer to maintain state
    if (this->FillFromBuffer(packet, pos)) {
        this->ShiftBuffer(receiveBuffer, pos);
        return SE_NOERR;
    }
    // we couldnt grab a whole packet, pull data from the OS and try and grab a packet again.
    SocketError err = this->Receive();
    pos = 0;
    err = (this->FillFromBuffer(packet, pos)) ? SE_NOERR : err;
    this->ShiftBuffer(receiveBuffer, pos);
    return err;
}


void TCPConnection::ShiftBuffer(vector<byte> & buffer, unsigned int nextToRead) {
    // theres nothing to do
    if (nextToRead <= 0) {
        return;
    }
    // how many bytes are left that need to be shifted to the front 
    unsigned int numToMove = buffer.size() - nextToRead;
    // shift them all
    for (unsigned int pos = 0; nextToRead < buffer.size();) {
        buffer[pos++] = buffer[nextToRead++];
    }
    // resize the vector so that we can add new data at buffer.size()
    buffer.resize(numToMove);
}


uint32_t TCPConnection::ReadHeader(const int start) {
    return NetToHost(receiveBuffer[start] << 24 |
        receiveBuffer[start + 1] << 16 |
        receiveBuffer[start + 2] << 8 |
        receiveBuffer[start + 3]);
}


SocketError TCPConnection::Receive(deque<Packet> & packets) {
    Packet p;                            // temporary packet
    unsigned int pos = 0;                // the position in the receive buffer
    SocketError err = this->Receive();   // grab a chunk of data
    // make as many packets as we can and put them in the vector.
    while (this->FillFromBuffer(p, pos)) {
        packets.push_back(p);
        p.clear();                      // make sure to erase the buffer
    }
    this->ShiftBuffer(receiveBuffer, pos); // shift the buffer once at the end
    return (packets.size() == 0) ? SE_NODATA : SE_NOERR;
}


SocketError TCPConnection::Send() {
    // do a raw send
    int bytesSent = this->Send(sendBuffer.data(), sendBuffer.size());
    // check for errors. 0 means the other end isn't connected
    if (bytesSent == 0) {         
        return SE_DISCONNECTED;
    } else if (bytesSent < 0) {     // if less than 0 there was some error so return it
        return this->GetError();
    }
    // move all the unsent (leftover) bytes to the front of the buffer
    unsigned int vpos, spos;
    for (vpos = bytesSent, spos = 0; vpos < sendBuffer.size(); ++spos, ++vpos) {
        sendBuffer[spos] = sendBuffer[vpos];
    }
    // change the size to be correct after the send
    sendBuffer.resize(sendBuffer.size() - bytesSent);
    return SE_NOERR;
}


SocketError TCPConnection::Receive() {
    // if the buffer is full and we havent gotten a packet the packet's too large.
    if (receiveBuffer.size() > MAX_PACKET_SIZE) {
        return SE_PACKETSIZE;
    }
    // Caclulate how much buffer space we have. If the buffer is small try to make it bigger
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();
    if (bytesAvail < FREE_THRESHOLD && receiveBuffer.capacity() < MAX_SOCKET_BUFSIZ) {
        receiveBuffer.reserve(receiveBuffer.capacity() * 2);
    }
    // save the old size since it's where we will append the first byte of new data
    uint32_t buffPosition = receiveBuffer.size();
    receiveBuffer.resize(receiveBuffer.capacity()); // expand the vector to as large as possible
    
    // receive everything we can. make sure we put new bytes starting at the end of the buffer
    int bytesRecvd = this->Recv(receiveBuffer.data() + buffPosition, bytesAvail);

    // check for errors
    if (bytesRecvd == 0) {          // 0 means the socket isnt connected anymore
        receiveBuffer.resize(buffPosition);
        return SE_DISCONNECTED;
    } else if (bytesRecvd < 0) {    // < 0 means there was some error
        receiveBuffer.resize(buffPosition);
        return this->GetError();
    }
    // resize the vector so that we can append at .size() next time
    receiveBuffer.resize(buffPosition + bytesRecvd);
    return SE_NOERR;
}


bool TCPConnection::FillFromBuffer(Packet & packet, unsigned int & pos) {
    // if we cant read the header (4 bytes) we cant do anytthing
    if (receiveBuffer.size() - pos < BYTES_IN_HEADER) {
        return false;
    }
    // read the header, it will tell us how many bytes in this "packet"
    unsigned int nextPacketSize = this->ReadHeader(pos);
    // if we cant get the whole packet do nothing
    if (nextPacketSize + BYTES_IN_HEADER > receiveBuffer.size() - pos) {
        return false;
    }
    // skip the header since its still in the buffer
    int endPacket = nextPacketSize + BYTES_IN_HEADER + pos;
    pos += BYTES_IN_HEADER;
    // read all the data bytes into the packet supplied
    for (; pos < endPacket; ++pos) {
        packet.push_back(receiveBuffer[pos]);
    }
    return true;
}


bool TCPConnection::WriteToBuffer(const Packet & packet) {
    // if theres nothing to send do nothing
    if (packet.size() == 0) {
        return false;
    }
    // if there's a problem with the packet's size return the PACKETSIZE error
    if (packet.size() > MAX_PACKET_SIZE || packet.size() < 0) {
        return false;
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
    return true;
}

// raw system calls
int TCPConnection::Send(byte* buffer, int size) {
#ifdef _WIN32
    return ::send(sock, reinterpret_cast<char*>(buffer), size, 0);
#else
    return ::send(sock, reinterpret_cast<void*>(buffer), size, 0);
#endif
}


int TCPConnection::Recv(byte* buffer, int size) {
#ifdef _WIN32
    return ::recv(sock, reinterpret_cast<char*>(buffer), size, 0);
#else
    return ::recv(sock, reinterpret_cast<void*>(buffer), size, 0);
#endif
}


void TCPConnection::PrintBuffer(vector<byte> & buffer, std::string msg) {
    int oldSize = buffer.size();
    buffer.resize(buffer.capacity());
    std::cout << msg << "\t";
    for (unsigned int i = 0; i < buffer.size(); ++i) {
        std::cout << std::to_string(buffer[i]) << " ";
    }
    std::cout << "\n";
    buffer.resize(oldSize);
}
