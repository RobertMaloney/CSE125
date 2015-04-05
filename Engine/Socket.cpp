#include "Socket.h"


/* ==================== Socket ===================== */


Gv::Socket::Socket() {
  initialized = false;
}


Gv::Socket::Socket(SocketAddress address) : sock(0), initialized(false) {
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


Gv::Socket::~Socket() {
  if (sock && initialized) {
    Close();
  }
}


void Gv::Socket::Close() {
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


void Gv::Socket::Bind() {
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


void Gv::Socket::Bind(SocketAddress addr) {
  this->address = addr;
  Bind();
}


void Gv::Socket::Connect(SocketAddress addr) {
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


bool Gv::Socket::IsInitialized() {
  return initialized;
}


void Gv::Socket::SetAddress(const string & ip) {
  inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
}


void Gv::Socket::SetAddress(const string & ip, unsigned short port) {
  memset((void*) &address, 0, sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  SetPortNo(port);
  SetAddress(ip);
}


void Gv::Socket::SetPortNo(unsigned short port) {
  address.sin_port = HostToNet(port);
}


unsigned short Gv::Socket::GetPort() {
  return address.sin_port;
}


unsigned long Gv::Socket::GetAddress() {
  return address.sin_addr.s_addr;
}


string Gv::Socket::GetPortStr() {
  return std::to_string(address.sin_port);
}


string Gv::Socket::GetAddressStr() {
  char str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, (void*) &address.sin_addr, str, INET_ADDRSTRLEN);
  return string(str);
}


/* ==================== SocketException ===================== */


Gv::SocketException::SocketException(int err)
: error(err) {
}


Gv::SocketException::SocketException(string msg) {
  this->errMsg = msg;
}


int Gv::SocketException::GetError() {
  return error;
}


const string & Gv::SocketException::GetErrMsg() {
  return this->errMsg;
}

