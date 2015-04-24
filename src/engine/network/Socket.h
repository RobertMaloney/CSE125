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

// Typedefs to try to mitigate different code styles
typedef sockaddr_in SocketAddress;
typedef addrinfo AddressInfo;

// Constants
const string DEFAULT_SERVER_IP("127.0.0.1");
const string DEFAULT_SERVER_PORT("1234");
const int INACTIVE_SOCKET = -1;
const int MAX_PACKET_SIZE = 65507;

// Platform independent error codes code sockets. 
enum SocketError {
    SE_NOERR,                 // default no error
    SE_WOULDBLOCK,            // the operation would block the thread
    SE_DISCONNECTED,          // the other end of the connection closed
    SE_BADFD,                 // the file descriptor(sock) is bad
    SE_NOCONNECT,             // couldnt connect to remote address
    SE_CONNECTED,             // the socket is already connected
    SE_ADDRINUSE,             // the address/port is already in use
    SE_PACKETSIZE,            // the packet was too big or too small 
    SE_INPROGRESS,            // an operation is already in progress
	SE_NODATA,
    SE_UNKNOWN                // everything else not really interested in them
};


class Socket {

public:

    virtual ~Socket();

    void close();            // close the socket belonging to this class
    void close(int fd);      // close the socket fd

    /* setNoDelay disables Nagle's algorithm that buffers bytes in the OS
     * so that packets sent use more bandwidth (more efficient). This is bad
     * for anything real-time since it can delay your message being sent
     */
    SocketError setNoDelay(bool on);

    /* setNonBlocking disables blocking operations. Instead if they can not
     * complete when called, they will return SE_WOULDBLOCK. 
     */
    SocketError setNonBlocking(bool on);

    string getErrorMsg();     // platform independent string error message
    static SocketError getError(); // gets the SocketError value that most recently occurred
    
    /* Initializes the socket layer (for windows) must be called once at the beginning 
     * of a program (not for each new socket)
     */
    static void initialize();   

    /* Performs an address lookup on the ip and port specified. sockType is usually either
     * SOCK_STREAM or SOCK_DGRAM (for TCP or UDP respectively). The function returns
     * a linked list of addrinfo structs containing address information about the 
     * parameters. NOTE: the return value must be freed after it is no longer needed
     * using freeaddrinfo();
     */
    static AddressInfo* dnsLookup(const string& ip, const string& port, int sockType);

protected:

    Socket();
    Socket(int sockfd);

    // Checks if the socket is valid (currently open)
    inline bool isSocketActive();

    // Set and Get the values of socket options
    SocketError setSockOpt(int optLevel, int optName, int val);
    int getSockOpt(int level, int optName);

    // Kills the program. Something really bad happened. Can provide a
    // string error message;
    void dieWithError(const string & errMsg = "");

    int sock;                      // socket fd for this class
    bool nonBlocking;              // is the socket in non-blocking mode
    SocketAddress remoteAddress;   // the address the socket is pointing to

};


class SocketException {

public:

    SocketException(int err);
    SocketException(string msg);

    int getError();
    const string & getErrMsg();

private:

    string errMsg;
    int error;

};


#endif
