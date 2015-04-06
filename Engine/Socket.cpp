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

#ifdef _WIN32
  if (bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#endif
}


void Blob::Socket::Connect(SocketAddress addr) {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

#ifdef _WIN32
  if (connect(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (connect(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#endif
}


bool Blob::Socket::IsInitialized() {
  return initialized;
}


void Blob::Socket::DNSLookup(const string & hostName, const string & port, 
  int type, struct addrinfo* res) {

  struct addrinfo hints;
  
  memset(static_cast<void*>(&hints), 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = type;

#ifdef _WIN32
  PADDRINFOA* result = reinterpret_cast<PADDRINFOA*>(res);
#else
  addrinfo* result = res;
#endif

  if (getaddrinfo(hostName.c_str(), port.c_str(), &hints, result) != 0) {
    throw SocketException("Error in dns lookup.\n");
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
    throw SocketException(WSAGetLastError());
  }
#else
  if(fcntl(sock, FSETFL, O_NONBLOCK) < 0) {
    Close();
    throw SocketException("Error setting to non-blocking.\n");
  }
#endif
}


void Blob::Socket::SetLocalAddress(const string & ip) {
  SocketAddress addr;
  memset(static_cast<void*>(&addr), 0, sizeof(SocketAddress));

  addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
  addr.sin_port = this->GetLocalAddress().sin_port;

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
  int size = sizeof(sockaddr_in);
  SocketAddress info;

  memset(static_cast<void*>(&info), 0, sizeof(SocketAddress));

#ifdef _WIN32
  if (getsockname(sock, reinterpret_cast<sockaddr*>(&info), &size) == SOCKET_ERROR ||
      size != sizeof(SocketAddress)) {
        Close();
        throw SocketException(WSAGetLastError());
  }
#else
  if (getsockname(sock, reinterpret_cast<sockaddr*>(&info), &size) < 0) || 
      size != sizeof(SocketAddress)) {
        Close();
        throw SocketException("Error retrieving port information.\n");
  }
#endif
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