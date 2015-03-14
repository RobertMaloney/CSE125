#include "Socket.h"


Gv::SocketException::SocketException(const string & err) 
  : error(err) {}


string Gv::SocketException::GetError() {
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
    WORD wVersionRequested = MAKEWORD(2, 0);
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
      throw SocketException("Socket already initialized.");
    }
  }
#endif
}


bool Gv::Socket::IsInitialized() {
  return initialized;
}

void Gv::Socket::Close() {
#ifdef _WIN32
  closesocket(static_cast<SOCKET>(socket));
#else
  close(socket);
#endif
  socket = 0;
}