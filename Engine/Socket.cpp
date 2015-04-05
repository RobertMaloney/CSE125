#include "Socket.h"


/* ==================== Socket ===================== */


Blob::Socket::Socket(void) : sock(0), initialized(false) {
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


Blob::Socket::~Socket(void) {
  if (sock && initialized) {
    Close();
  }
}


void Blob::Socket::Close(void) {
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


void Blob::Socket::Bind(SocketAddress addr) {
  if (!initialized) {
    throw SocketException("Socket not initialized.\n");
  }

#ifdef _WIN32
  if (bind(sock, (const sockaddr*) &addr, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (bind(sock, (sockaddr*) &addr, sizeof(SocketAddress)) < 0) {
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
  if (connect(sock, (const sockaddr*) &addr, sizeof(SocketAddress)) == SOCKET_ERROR) {
    Close();
    throw SocketException(WSAGetLastError());
  }
#else
  if (connect(sock, (sockaddr*) &addr, sizeof(SocketAddress)) < 0) {
    Close();
    throw SocketException("Failed to bind.\n");
  }
#endif
}


bool Blob::Socket::IsInitialized(void) {
  return initialized;
}


void Blob::Socket::SetAddress(const string & ip) {
  SocketAddress addr;
  memset(static_cast<void*>(&addr), 0, sizeof(SocketAddress));
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
  addr.sin_port = this->GetSockName().sin_port;
  Bind(addr);
}


void Blob::Socket::SetPortNo(unsigned short port) {
  SocketAddress addr;
  memset(static_cast<void*>(&addr), 0, sizeof(SocketAddress));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = this->GetSockName().sin_addr.s_addr;
  addr.sin_port = HostToNet(port);
  Bind(addr);
}


Blob::SocketAddress Blob::Socket::GetSockName(void) {
  int size = sizeof(sockaddr_in);
  SocketAddress info;

  memset(static_cast<void*>(&info), 0, sizeof(SocketAddress));

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


unsigned short Blob::Socket::GetPort(void) {
  return NetToHost(this->GetSockName().sin_port);
}


unsigned long Blob::Socket::GetAddress(void) {
  return NetToHost(static_cast<long>(this->GetSockName().sin_addr.s_addr));
}


string Blob::Socket::GetPortStr(void) {
  return std::to_string(NetToHost(this->GetSockName().sin_port));
}


string Blob::Socket::GetAddressStr(void) {
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


int Blob::SocketException::GetError(void) {
  return error;
}


const string & Blob::SocketException::GetErrMsg(void) {
  return this->errMsg;
}

