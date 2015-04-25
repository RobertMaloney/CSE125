#include "Socket.h"


/* ==================== Socket ===================== */

// Constructors
Socket::Socket() : sock(INACTIVE_SOCKET), nonBlocking(false) {}


Socket::Socket(int sockfd) : sock(sockfd), nonBlocking(false) {}


// Destructor
Socket::~Socket() {
    this->close();
}


void Socket::close() {
    if (!this->isSocketActive()) {
        return;
    }
#ifdef _WIN32
    ::closesocket(sock);
#else
    ::close(sock);
#endif
    sock = -1;
}


void Socket::close(int fd) {
    if (fd == INACTIVE_SOCKET) {
        return;
    }
#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}


SocketError Socket::setNoDelay(bool on) {
    if (!this->isSocketActive()) {           // cant change options on inactive sockets
        return SE_BADFD;
    }
    return this->setSockOpt(SOL_SOCKET, TCP_NODELAY, on ? 1 : 0);
}


SocketError Socket::setNonBlocking(bool on) {
    if (!this->isSocketActive()) {            // cant change options on inactive sockets
        return SE_BADFD;
    }
#ifdef _WIN32
    u_long flags = on ? 1 : 0;
    if (::ioctlsocket(sock, FIONBIO, &flags) != NO_ERROR) {
        return this->getError();
    }
#else
    int flags = on ? 1 : 0;
    if(ioctl(sock, FIONBIO, &flags) != 0) {
        return this->getError();
    }
#endif
    nonBlocking = on;     
    return SE_NOERR;
}


SocketError Socket::getError() {
#ifdef _WIN32
    switch (WSAGetLastError()) {
    case WSAEWOULDBLOCK:
        return SE_WOULDBLOCK;
    case WSAEBADF:
        return SE_BADFD;
    case WSAEINPROGRESS:
        return SE_INPROGRESS;
    case WSAEALREADY:
        return SE_INPROGRESS;
    case WSAENOTSOCK:
        return SE_BADFD;
    case WSAEMSGSIZE:
        return SE_PACKETSIZE;
    case WSAEADDRINUSE:
        return SE_ADDRINUSE;
    case WSAECONNABORTED:
        return SE_DISCONNECTED;
    case WSAENETRESET:
        return SE_DISCONNECTED;
    case WSAECONNRESET:
        return SE_DISCONNECTED;
    case WSAESTALE:
        return SE_BADFD;
    case WSAECONNREFUSED:
        return SE_NOCONNECT;
    case WSAETIMEDOUT:
        return SE_NOCONNECT;
    case WSAESHUTDOWN:
        return SE_BADFD;
    case WSAENOTCONN:
        return SE_DISCONNECTED;
    case WSAEISCONN:
        return SE_CONNECTED;
    default:
        return SE_UNKNOWN;
    }
#else
    return errno;
#endif
}


string Socket::getErrorMsg() {
#ifdef _WIN32
    int err = WSAGetLastError();             // gets the last error on windows (like errno)
    LPSTR errString = NULL;
    // this monstrous function call will give us a system error message
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &errString, 0, NULL);

    string result = string(errString) + "\n";
    LocalFree(errString);                    // we have to free the buffer the string was put in
    return result;
#else
    return string(strerror(errno)) + "\n";
#endif
}


inline bool Socket::isSocketActive() {
    return sock != INACTIVE_SOCKET;          // INACTIVE_SOCKET is -1. 
}


SocketError Socket::setSockOpt(int optLevel, int optName, int val) {
#ifdef _WIN32
    const char opt = static_cast<char>(val);
#else
    int opt = val;
#endif
    if(::setsockopt(sock, optLevel, optName, &opt, sizeof(opt)) != 0){
        return this->getError();
    }
    return SE_NOERR;
}


int Socket::getSockOpt(int optLevel, int optName) {
#ifdef _WIN32
    char opt = 0;
#else
    int opt = 0;
#endif
    int optLen = sizeof(opt);
    if (::getsockopt(sock, optLevel, optName, &opt, &optLen) != 0) {
        return -1;
    }
    return opt;
}


/* This only needs to be called once at the beginning of a program on windows. 
 */
void Socket::initialize() {
#ifdef _WIN32
    WSAData wsaData;
    WORD wsaVerRequested = MAKEWORD(2, 2);
    if (WSAStartup(wsaVerRequested, &wsaData) != 0) {    // start the socket layer
        throw SocketException("Unable to initialize socket layer.");
    }
#endif
}


AddressInfo* Socket::dnsLookup(const string& ip, const string& port, int sockType) {
    AddressInfo hints;
    AddressInfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = sockType; // whatever type of socket you want (SOCK_STREAM or SOCK_DGRAM)

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo) != 0) {
		if (servinfo){
			freeaddrinfo(servinfo);
		}
        return nullptr;
    }
    return servinfo;
}


void Socket::dieWithError(const string & errMsg) {
    cerr << errMsg << "\tSYSTEM ERROR: " << this->getErrorMsg() << endl;
    this->close();
    throw SocketException(this->getError());
}



/* ==================== SocketException ===================== */


SocketException::SocketException(int err)
: error(err) {
}


SocketException::SocketException(string msg) {
    this->errMsg = msg;
}


int SocketException::getError() {
    return error;
}


const string & SocketException::getErrMsg() {
    return this->errMsg;
}
