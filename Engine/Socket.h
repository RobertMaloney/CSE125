#ifndef GV_SOCKET_H
#define GV_SOCKET_H

#ifdef _WIN32

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>

#endif

#include "ByteBuffer.h"

#include <stdexcept>
#include <string>

using std::string;

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

    SocketException(const std::string & error);

    string GetError();

  private:

    string error;

  };

}
#endif