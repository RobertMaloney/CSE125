#include "Socket.h"


Gv::SocketException::SocketException(int err) 
  : error(err) {}


int Gv::SocketException::GetError() {
  return error;
}


Gv::Socket::Socket() {}


Gv::Socket::Socket(SocketAddress address) : sock(0), initialized(false) {
#ifdef _WIN32
  if (!initialized) {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
      throw SocketException(WSAGetLastError());
    }
  }
#endif
  initialized = true;
}


Gv::Socket::~Socket() {
  if (sock) {
    Close();
  }
}


bool Gv::Socket::IsInitialized() {
  return initialized;
}


void Gv::Socket::Close() {
#ifdef _WIN32
  closesocket(static_cast<SOCKET>(sock));
  WSACleanup();
#else
  close(sock);
#endif
  sock = 0;
}