#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include "Socket.h"
#include "TCPConnection.h"


class TCPListener : public Socket {

public:

    TCPListener();
    ~TCPListener();

    bool Bind(const string & ip, const string & port);
    void Listen(int maxPending);
    TCPConnection* Accept();

private:

    SocketAddress listenAddr;
    bool isListening;
	//$(OutDir)$(TargetName)$(TargetExt)
};

#endif