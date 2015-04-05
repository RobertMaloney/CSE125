#include "UdpSocket.h"


Gv::UdpSocket::UdpSocket(SocketAddress addr){
  this->address = addr;
}


Gv::UdpSocket::~UdpSocket() {

}


void Gv::UdpSocket::Initialize() {
  if (!initialized) {
    cerr << "Initialize called after invalid socket construction." 
         << " Address : " << this << endl;
    return;
  }

  sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
}


void Gv::UdpSocket::Send(Packet* packet) {
  if (!packet) {
    cerr << "Null packet to send. Address : " << this << endl;
    return;
  }


}


Gv::Packet* Gv::UdpSocket::Receive() {
  return nullptr;
}


