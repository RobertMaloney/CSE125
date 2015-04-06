#include "Socket.h"


/* ==================== Socket ===================== */


Blob::Socket::Socket() : sock(0), initialized(false) {
#ifdef _WIN32
  if (!initialized) {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
      throw SocketException(WSAGetLastError());
    }
  }
#endif
}


Blob::Socket::~Socket() {
  if (sock && initialized) {
    Close();
  }
}


void Blob::Socket::Close() {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }
#ifdef _WIN32
  closesocket(sock);
  WSACleanup();
#else
  ::close(sock);
#endif
  sock = -1;
  initialized = false;
}


void Blob::Socket::Bind(SocketAddress addr) {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

  if (bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind. " + this->GetError());
  }
}


void Blob::Socket::Connect(SocketAddress addr) {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

  if (connect(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to Connect. " + this->GetError());
  }
}


bool Blob::Socket::IsInitialized() {
  return initialized;
}


/*
 * If you use this function remember that you must free res when you are done 
 * with it using freeaddrinfo()
 */
void Blob::Socket::DNSLookup(const string & hostName, const string & port, 
  int type, struct addrinfo** res) {

  struct addrinfo hints;
  
  memset(static_cast<void*>(&hints), 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = type;

  if (getaddrinfo(hostName.c_str(), port.c_str(), &hints, res) != 0) {
    freeaddrinfo(*res);
    throw SocketException("Error in dns lookup.");
  }
}


void Blob::Socket::SetNonBlocking() {
  if (!initialized) {
    throw SocketException("Unitialized socket set non-blocking.\n");
  }

#ifdef _WIN32
  u_long argp = 0;

  if (ioctlsocket(sock, FIONBIO, &argp) != NO_ERROR) {
    Close();
    throw SocketException("Could not set non-blocking. " + this->GetError());
  }
#else
  if(fcntl(sock, FSETFL, O_NONBLOCK) < 0) {
    Close();
    throw SocketException("Error setting to non-blocking. " + this->GetError());
  }
#endif
}


void Blob::Socket::SetLocalAddress(const string & ip) {
  SocketAddress addr;
  memset(static_cast<void*>(&addr), 0, sizeof(SocketAddress));

  addr.sin_family = AF_INET;
  addr.sin_port = this->GetLocalAddress().sin_port;
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

  Bind(addr);
}


void Blob::Socket::SetLocalPort(unsigned short port) {
  SocketAddress addr;
  memset(static_cast<void*>(&addr), 0, sizeof(SocketAddress));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = this->GetLocalAddress().sin_addr.s_addr;
  addr.sin_port = HostToNet(port);

  Bind(addr);
}


Blob::SocketAddress Blob::Socket::GetLocalAddress() {
  SocketAddress info;
  int size = sizeof(sockaddr_in);

  memset(static_cast<void*>(&info), 0, sizeof(SocketAddress));

  if (getsockname(sock, reinterpret_cast<sockaddr*>(&info), &size) < 0 
      || size != sizeof(SocketAddress)) {
        Close();
        throw SocketException("Error retrieving port information. " + this->GetError());
  }
  return info;
}


string Blob::Socket::GetLocalPortStr() {
  return std::to_string(NetToHost(this->GetLocalAddress().sin_port));
}


string Blob::Socket::GetLocalAddressStr() {
  char str[INET_ADDRSTRLEN];
  SocketAddress info = this->GetLocalAddress();

  inet_ntop(AF_INET, static_cast<void*>(&info.sin_addr), str, INET_ADDRSTRLEN);

  return string(str);
}



string Blob::Socket::GetError() {
#ifdef _WIN32
  int err = WSAGetLastError();
  LPSTR errString = NULL;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS, 
    NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
    (LPSTR) &errString, 0, NULL);

  string result = string(errString);
  LocalFree(errString);

  return result;
#else
  return string(strerror(errno));
#endif
}

/* ==================== SocketException ===================== */


Blob::SocketException::SocketException(int err)
: error(err) {
}


Blob::SocketException::SocketException(string msg) {
  this->errMsg = msg;
}


int Blob::SocketException::GetError() {
  return error;
}


const string & Blob::SocketException::GetErrMsg() {
  return this->errMsg;
}


// Just here so the project will compile
int main(int argc, char* argv[]) {
  return 0;
}