#ifndef GV_SOCKET_H
#define GV_SOCKET_H

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <stdexcept>
#include <string>
#include "ByteBuffer.h"


namespace Gv {

  typedef ByteBuffer Packet;
  
  class Socket {

  public:

    Socket();
    virtual ~Socket();

    virtual void Initialize();
    virtual void Send(Packet* packet) = 0;
    virtual Packet* Receive() = 0;

    void Close();
    bool IsInitialized();

  protected:

    int socket;
    bool initialized;

  };


  class SocketException {

  public:

    SocketException(int err);

    int GetError();

  private:

    int error;

  };

}
#endif