#ifndef BLOB_SOCKET_H
#define BLOB_SOCKET_H

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

    Socket(void);
    virtual ~Socket(void);

    void Close();
    void Bind(SocketAddress addr);
    void Connect(SocketAddress addr);

    bool IsInitialized(void);
    virtual void Initialize(void) = 0;

    virtual void Send(Packet* packet) = 0;
    virtual Packet* Receive(void) = 0;

    void SetAddress(const string & ip);
    void SetPortNo(unsigned short port);

    unsigned short GetPort(void);
    unsigned long GetAddress(void);

    string GetPortStr(void);
    string GetAddressStr(void);

    SocketAddress GetSockName(void);

  protected:

    int sock;
    bool initialized;

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