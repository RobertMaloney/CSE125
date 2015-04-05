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
    return;
  }

#ifdef _WIN32
  if (bind(sock, (const sockaddr*) &address, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#else
  if (bind(sock, (sockaddr*) &address, sizeof(SocketAddress)) < 0) {
    close(sock);
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
    return;
  }

  this->address = addr;

#ifdef _WIN32
  if (connect(sock, (const sockaddr*) &address, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#else
  if (connect(sock, (sockaddr*) &address, sizeof(SocketAddress)) < 0) {
    close(sock);
    throw SocketException("Failed to bind.\n");
  }
#endif
}


bool Blob::Socket::IsInitialized() {
  return initialized;
}


void Blob::Socket::SetAddress(const string & ip) {
  inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
}


void Blob::Socket::SetAddress(const string & ip, unsigned short port) {
  memset((void*) &address, 0, sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  SetPortNo(port);
  SetAddress(ip);
}


void Blob::Socket::SetPortNo(unsigned short port) {
  address.sin_port = HostToNet(port);
}


unsigned short Blob::Socket::GetPort() {
  return address.sin_port;
}


unsigned long Blob::Socket::GetAddress() {
  return address.sin_addr.s_addr;
}


string Blob::Socket::GetPortStr() {
  return std::to_string(address.sin_port);
}


string Blob::Socket::GetAddressStr() {
  char str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, (void*) &address.sin_addr, str, INET_ADDRSTRLEN);
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

