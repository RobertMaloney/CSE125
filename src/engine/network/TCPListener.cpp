#include "TCPListener.h"

TCPListener::TCPListener() {}


TCPListener::~TCPListener() {
    this->Close();
}

SocketError TCPListener::Bind(const string & ip, const string & port) {
    AddressInfo* iter = this->DNSLookup(ip, port, SOCK_STREAM);

    for (; iter; iter = iter->ai_next) {
        sock = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (sock == INACTIVE_SOCKET) {
            continue;
        }
        if (this->SetSockOpt(SOL_SOCKET, SO_REUSEADDR, 1) != 0) {
            this->Close(sock);
            continue;
        }
        if (::bind(sock, iter->ai_addr, iter->ai_addrlen) != 0) {
            this->Close(sock);
            continue;
        }
        break;
    }
    
    if (!iter) { 
        return SE_UNKNOWN; 
    }

    memcpy(static_cast<void*>(&this->listenAddr), static_cast<void*>(iter->ai_addr), sizeof(SocketAddress));
    freeaddrinfo(iter);
    return SE_NOERR;
}

void TCPListener::Listen(int maxPending) {
    if (!this->IsSocketActive()) {
        this->DieWithError("Tring to listen on inactive socket.");
    }

    if (::listen(sock, maxPending) != 0) {
        this->DieWithError("Error in listening in Listen.");
    }

    isListening = true;
}


TCPConnection* TCPListener::Accept() {
    if (!this->isListening) {
        this->DieWithError("Trying to accept while not listening.");
    }

    int clientFd = 0;
    int size = sizeof(SocketAddress);

    clientFd = ::accept(sock, reinterpret_cast<sockaddr*>(&remoteAddress), &size);

    if (clientFd < 0) {
        cerr << "Error while accepting client. " + this->GetErrorMsg() << endl;
        return nullptr;
    }
    return new TCPConnection(clientFd, remoteAddress);
}