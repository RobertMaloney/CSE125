#ifndef GV_SOCKET_H
#define GV_SOCKET_H

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <stdexcept>
#include <string>
#include <exception>

#include "ByteBuffer.h"
#include "Endianness.h"

using std::string;

namespace Blob {

  typedef ByteBuffer Packet;
  typedef sockaddr_in SocketAddress;

  class Socket {

  public:

    Socket(SocketAddress addr);
    Socket();
    virtual ~Socket();

    void Close();
    void Bind();
    void Bind(SocketAddress addr);
    void Connect(SocketAddress addr);

    bool IsInitialized();
    virtual void Initialize() = 0;

    virtual void Send(Packet* packet) = 0;
    virtual Packet* Receive() = 0;

    void SetAddress(const string & ip);
    void SetAddress(const string & ip, unsigned short port);
    void SetPortNo(unsigned short port);

    unsigned short GetPort();
    unsigned long GetAddress();

    string GetPortStr();
    string GetAddressStr();

  protected:

    int sock;
    bool initialized;
    SocketAddress address;

  };


  class SocketException {

  public:

    SocketException(int err);
    SocketException(string msg);

    int GetError();
    const string & GetErrMsg();

  private:

    string errMsg;
    int error;

  };

}
#endif