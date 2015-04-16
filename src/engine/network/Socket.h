#ifndef SOCKET_H
#define SOCKET_H

// Platform includes
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

// C++ includes
#include <stdexcept>
#include <string>
#include <iostream>

// Engine includes
#include "Endianness.h"

using std::string;
using std::cerr;
using std::endl;


typedef sockaddr_in SocketAddress;
typedef addrinfo AddressInfo;

// Constants
const string DEFAULT_SERVER_IP("127.0.0.1");
const string DEFAULT_SERVER_PORT("1234");
const int INACTIVE_SOCKET = -1;

class Socket {

public:

    virtual ~Socket();

    void Close();
    void Close(int fd);

    void SetNoDelay(bool on);

    int GetDescriptor();
    int GetError();
    string GetErrorMsg();

    static AddressInfo* DNSLookup(const string& ip, const string& port, int sockType);

protected:

    Socket();
    Socket(int sockfd);

    void Initialize();
    inline bool IsSocketActive();

    int SetSockOpt(int optLevel, int optName, int val);
    int GetSockOpt(int level, int optName);

    void DieWithError(const string & errMsg);

    int sock;
	bool nonBlocking;
    SocketAddress remoteAddress;

    friend class SocketSelector;

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


#endif
