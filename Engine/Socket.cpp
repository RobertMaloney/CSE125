#include "Socket.h"


/* ==================== Socket ===================== */


Blob::Socket::Socket() {
  initialized = false;
}


Blob::Socket::Socket(SocketAddress address) : sock(0), initialized(false) {
#ifdef _WIN32
  if (!initialized) {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
      WSACleanup();
      throw SocketException(WSAGetLastError());
    }
  }
#endif
  initialized = true;
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
  closesocket(static_cast<SOCKET>(sock));
  WSACleanup();
#else
  close(sock);
#endif
  sock = -1;
}


void Blob::Socket::Bind() {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

#ifdef _WIN32
  if (bind(sock, (const sockaddr*) &address, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (bind(sock, (sockaddr*) &address, sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#endif
}


void Blob::Socket::Bind(SocketAddress addr) {
  this->address = addr;
  Bind();
}


void Blob::Socket::Connect(SocketAddress addr) {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

  this->address = addr;

#ifdef _WIN32
  if (connect(sock, (const sockaddr*) &address, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (connect(sock, (sockaddr*) &address, sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#endif
}


bool Blob::Socket::IsInitialized() {
  return initialized;
}


void Blob::Socket::SetAddress(const string & ip) {
  inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
  Bind();
}


void Blob::Socket::SetAddress(const string & ip, unsigned short port) {
  memset((void*) &address, 0, sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  SetPortNo(port);
  SetAddress(ip);
  Bind();
}


void Blob::Socket::SetPortNo(unsigned short port) {
  address.sin_port = HostToNet(port);
  Bind();
}


Blob::SocketAddress Blob::Socket::GetSockName() {
  int size = sizeof(sockaddr_in);
  SocketAddress info;

#ifdef _WIN32
  if (getsockname(sock, (sockaddr*) &info, &size) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (getsockname(sock, (sockaddr*) &info, &size) < 0) {
    Close();
    throw SocketException("Error retrieving port information.\n");
  }
#endif
  return info;
}


unsigned short Blob::Socket::GetPort() {
  return NetToHost(this->GetSockName().sin_port);
}


unsigned long Blob::Socket::GetAddress() {
  return NetToHost(static_cast<long>(this->GetSockName().sin_addr.s_addr));
}


string Blob::Socket::GetPortStr() {
  return std::to_string(NetToHost(this->GetSockName().sin_port));
}


string Blob::Socket::GetAddressStr() {
  char str[INET_ADDRSTRLEN];
  SocketAddress info = this->GetSockName();
  inet_ntop(AF_INET, (void*) &info.sin_addr, str, INET_ADDRSTRLEN);
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

