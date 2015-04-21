#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include "Socket.h"
#include "TCPConnection.h"


class TCPListener : public Socket {

public:

    TCPListener();
    ~TCPListener();

    /* Bind this socket to the ip and port specified. This means it will
     * use the ip and port given. Ip must be in IPv4 format (ex. 127.0.0.1)
     * Port must be in the range 1023 < port < 65507
     */
    SocketError Bind(const string & ip, const string & port);

    /* Start listening to the socket we are bound to. Must call bind before
     * listen. Maxpending is the number of connections that can be buffered
     * in the OS waiting to connect to this socket.
     */
    void Listen(int maxPending);

    /* Accept will pull a waiting entity from the buffer that holds waiters
     * in the os. It will then create a TCPConnection and return it. The connection
     * belongs to the caller of accept you must free the memory. If an error
     * occurs Accept will return a nullptr.
     */
    TCPConnection* Accept();

private:

    SocketAddress listenAddr;
    bool isListening;

};

#endif