#include "Socket.h"


/* ==================== Socket ===================== */

// Constructors
Socket::Socket() : sock(INACTIVE_SOCKET) {}


Socket::Socket(int sockfd) : sock(sockfd) {}


// Destructor
Socket::~Socket() {
    this->Close();
}


void Socket::Close() {
    if (!this->IsSocketActive()) {
        return;
    }
#ifdef _WIN32
    ::closesocket(sock);
#else
    ::close(sock);
#endif
    sock = -1;
}


void Socket::Close(int fd) {
    if (fd == INACTIVE_SOCKET) {
        return;
    }
#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}


SocketError Socket::SetNoDelay(bool on) {
    if (!this->IsSocketActive()) {           // cant change options on inactive sockets
        return SE_BADFD;
    }
    return this->SetSockOpt(SOL_SOCKET, TCP_NODELAY, on ? 1 : 0);
}


SocketError Socket::SetNonBlocking(bool on) {
    if (!this->IsSocketActive()) {            // cant change options on inactive sockets
        return SE_BADFD;
    }
#ifdef _WIN32
    u_long flags = on ? 1 : 0;
    if (::ioctlsocket(sock, FIONBIO, &flags) != NO_ERROR) {
        return this->GetError();
    }
#else
    int flags = on ? 1 : 0;
    if(ioctl(sock, FIONBIO, &flags) != 0) {
        return this->GetError();
    }
#endif
    nonBlocking = on;     
    return SE_NOERR;
}


SocketError Socket::GetError() {
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


string Socket::GetErrorMsg() {
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


inline bool Socket::IsSocketActive() {
    return sock != INACTIVE_SOCKET;          // INACTIVE_SOCKET is -1. 
}


SocketError Socket::SetSockOpt(int optLevel, int optName, int val) {
#ifdef _WIN32
    const char opt = static_cast<char>(val);
#else
    int opt = val;
#endif
    if(setsockopt(sock, optLevel, optName, &opt, sizeof(opt)) != 0){
        return this->GetError();
    }
    return SE_NOERR;
}


int Socket::GetSockOpt(int optLevel, int optName) {
#ifdef _WIN32
    char opt = 0;
#else
    int opt = 0;
#endif
    int optLen = sizeof(opt);
    if (getsockopt(sock, optLevel, optName, &opt, &optLen) != 0) {
        return -1;
    }
    return opt;
}


/* This only needs to be called once at the beginning of a program on windows. 
 */
void Socket::Initialize() {
#ifdef _WIN32
    WSAData wsaData;
    WORD wsaVerRequested = MAKEWORD(2, 2);
    if (WSAStartup(wsaVerRequested, &wsaData) != 0) {    // start the socket layer
        throw SocketException("Unable to initialize socket layer.");
    }
#endif
}


AddressInfo* Socket::DNSLookup(const string& ip, const string& port, int sockType) {
    AddressInfo hints;
    AddressInfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = sockType; // whatever type of socket you want (SOCK_STREAM or SOCK_DGRAM)

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo) != 0) {
		std::cout << WSAGetLastError() << std::endl;
		if (servinfo){
			freeaddrinfo(servinfo);
		}
        return nullptr;
    }
    return servinfo;
}


void Socket::DieWithError(const string & errMsg) {
    cerr << errMsg << "\tSYSTEM ERROR: " << this->GetErrorMsg() << endl;
    this->Close();
    throw SocketException(this->GetError());
}



/* ==================== SocketException ===================== */


SocketException::SocketException(int err)
: error(err) {
}


SocketException::SocketException(string msg) {
    this->errMsg = msg;
}


int SocketException::GetError() {
    return error;
}


const string & SocketException::GetErrMsg() {
    return this->errMsg;
}
