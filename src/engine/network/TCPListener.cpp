#include "TCPListener.h"

TCPListener::TCPListener() : Socket() {}


TCPListener::~TCPListener() {
    this->close();
}

SocketError TCPListener::bind(const string & ip, const string & port) {
    AddressInfo* iter = this->dnsLookup(ip, port, SOCK_STREAM);

    for (; iter; iter = iter->ai_next) {
        sock = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (sock == INACTIVE_SOCKET) {
            continue;
        }
        if (this->setSockOpt(SOL_SOCKET, SO_REUSEADDR, 1) != 0) {
            this->close(sock);
            continue;
        }
        if (::bind(sock, iter->ai_addr, iter->ai_addrlen) != 0) {
            this->close(sock);
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

void TCPListener::listen(int maxPending) {
    if (!this->isSocketActive()) {
        this->dieWithError("Tring to listen on inactive socket.");
    }

    if (::listen(sock, maxPending) != 0) {
        this->dieWithError("Error in listening in Listen.");
    }

    isListening = true;
}


TCPConnection* TCPListener::accept() {
    if (!this->isListening) {
        this->dieWithError("Trying to accept while not listening.");
    }

    int clientFd = 0;
    int size = sizeof(SocketAddress);

    clientFd = ::accept(sock, reinterpret_cast<sockaddr*>(&remoteAddress), &size);

    if (clientFd < 0) {
		if (!nonBlocking){
			cerr << "Error while accepting client. " + this->getErrorMsg() << endl;
		}
        return nullptr;
    }
    return new TCPConnection(clientFd, remoteAddress);
}