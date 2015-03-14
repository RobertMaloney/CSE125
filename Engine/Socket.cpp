#include "Socket.h"


Gv::SocketException::SocketException(int err) 
  : error(err) {}


int Gv::SocketException::GetError() {
  return error;
}


Gv::Socket::Socket() : socket(0), initialized(false) {}


Gv::Socket::~Socket() {
  if (socket) {
    Close();
  }
}


void Gv::Socket::Initialize() {
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


bool Gv::Socket::IsInitialized() {
  return initialized;
}

void Gv::Socket::Close() {
#ifdef _WIN32
  closesocket(static_cast<SOCKET>(socket));
  WSACleanup();
#else
  close(socket);
#endif
  socket = 0;
}