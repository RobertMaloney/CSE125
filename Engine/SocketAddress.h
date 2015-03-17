#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

class SocketAddress {

public:

  SocketAddress();
  ~SocketAddress();

  unsigned short GetPort();
  unsigned int GetAddress();

private:

  sockaddr_in address;

};


#endif