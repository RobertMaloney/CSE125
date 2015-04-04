#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <string>

#include "ByteOrder.h"

using std::string;

namespace Gv {

  class SocketAddress {

  public:

    SocketAddress();
    SocketAddress(const string & ip, unsigned short port);
    ~SocketAddress();

    unsigned short GetPort();
    unsigned long GetAddress();

    string GetStrPort();
    string GetStrAddress();

  private:

    sockaddr_in address;

    friend class Socket;
    friend class UdpSocket;

  };

}

#endif