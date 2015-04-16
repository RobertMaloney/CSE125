#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H


#include "Socket.h"


class TCPConnection : public Socket {

public:

    TCPConnection();
    TCPConnection(int sockFd, SocketAddress remAddr);
    ~TCPConnection();

    int Connect(const string & ip, const string & port);

    int Send(const void* data, int size);
    int Receive(void* buffer, int buffSize);

};

#endif