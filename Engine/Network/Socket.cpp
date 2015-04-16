#include "Socket.h"


/* ==================== Socket ===================== */


Socket::Socket() : sock(INACTIVE_SOCKET) {
    this->Initialize();
	nonBlocking = false;
}


Socket::Socket(int sockfd) : sock(sockfd) {
    this->Initialize();
	nonBlocking = false;
}


Socket::~Socket() {
    if (this->IsSocketActive()) {
        this->Close();
    }
}


void Socket::Close() {
    if (!this->IsSocketActive()) {
        this->DieWithError("Can not close inactive socket.");
    }
#ifdef _WIN32
    ::closesocket(sock);
    WSACleanup();
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


void Socket::SetNoDelay(bool on) {
    if (!this->IsSocketActive()) {
        this->DieWithError("Trying to set inactive socket to no delay.");
    }
    this->SetSockOpt(SOL_SOCKET, TCP_NODELAY, on ? 1 : 0);
}


int Socket::GetDescriptor() {
    return sock;
}


int Socket::GetError() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}


string Socket::GetErrorMsg() {
#ifdef _WIN32
    int err = WSAGetLastError();
    LPSTR errString = NULL;
    // lol this is one function call
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &errString, 0, NULL);

    string result = string(errString) + "\n";
    LocalFree(errString);
    return result;
#else
    return string(strerror(errno)) + "\n";
#endif
}


void Socket::Initialize() {
#ifdef _WIN32
    WSAData wsaData;
    WORD wsaVerRequested = MAKEWORD(2, 2);
    if (WSAStartup(wsaVerRequested, &wsaData) != 0) {
        this->DieWithError("Error in WSAStartup.");
    }
  
#endif
}


inline bool Socket::IsSocketActive() {
    return sock != INACTIVE_SOCKET;
}


int Socket::SetSockOpt(int optLevel, int optName, int val) {
#ifdef _WIN32
    const char opt = static_cast<char>(val);
#else
    int opt = val;
#endif
    return setsockopt(sock, optLevel, optName, &opt, sizeof(opt));
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


AddressInfo* Socket::DNSLookup(const string& ip, const string& port, int sockType) {
    AddressInfo hints;
    AddressInfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = sockType;

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo) != 0) {
        freeaddrinfo(servinfo);
        throw SocketException("Getaddrinfo failed in listen(). \n");
    }
    return servinfo;
}


void Socket::DieWithError(const string & errMsg) {
    cerr << errMsg << "\tSYSTEM ERROR: " << this->GetErrorMsg() << endl;
    if (this->IsSocketActive()) {
        this->Close();
    }
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
