#ifndef BLOB_SOCKET_H
#define BLOB_SOCKET_H

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <stdexcept>
#include <string>
#include <exception>
#include <mutex>

#include "ByteBuffer.h"
#include "Endianness.h"

using std::string;


namespace Blob {


  typedef ByteBuffer Packet;
  typedef sockaddr_in SocketAddress;


  class Socket {

  public:

    Socket();
    virtual ~Socket();

    void Close();
    void Bind(SocketAddress addr);
    void Connect(SocketAddress addr);

    bool IsInitialized();
    virtual void Initialize() = 0;

    virtual void Send(Packet* packet) = 0;
    virtual Packet* Receive() = 0;

    static void DNSLookup(const string & hostName, const string & port, 
                          int type, struct addrinfo** res);

    void SetNonBlocking();

    void SetLocalAddress(const string & ip);
    void SetLocalPort(unsigned short port);

    string GetLocalPortStr();
    string GetLocalAddressStr();

    SocketAddress GetLocalAddress();

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