#include "TCPConnection.h"


TCPConnection::TCPConnection() {
    this->Initialize();
}


TCPConnection::TCPConnection(int sockFd, SocketAddress remAddr) {
    this->Initialize();
    this->sock = sockFd;
    this->remoteAddress = remAddr;
}


TCPConnection::~TCPConnection() {
    this->Close();
}


int TCPConnection::Connect(const string & ip, const string & port) {
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
    if (iter != NULL) {
        freeaddrinfo(iter);
    }
    return sock != INACTIVE_SOCKET;
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