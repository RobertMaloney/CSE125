#include "SocketAddress.h"


Gv::SocketAddress::SocketAddress() {}


Gv::SocketAddress::SocketAddress(const string & ip, unsigned short port) {
  memset((void*) &address, 0, sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  address.sin_port = Gv::ByteOrder::HostToNet(port);
  inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
}


Gv::SocketAddress::~SocketAddress() {}


unsigned short Gv::SocketAddress::GetPort() {
  return address.sin_port;
}


unsigned long Gv::SocketAddress::GetAddress() {
  return address.sin_addr.s_addr;
}


string Gv::SocketAddress::GetStrPort() {
  return std::to_string(address.sin_port);
}


string Gv::SocketAddress::GetStrAddress() {
  char str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, (void*) &address.sin_addr, str, INET_ADDRSTRLEN);
  return string(str);
}
