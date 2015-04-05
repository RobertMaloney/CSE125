#include "UdpSocket.h"


Blob::UdpSocket::UdpSocket(SocketAddress addr){

}


//TODO
Blob::UdpSocket::~UdpSocket(void) {

}


// TODO
void Blob::UdpSocket::Initialize(void) {
  if (!initialized) {
    cerr << "Initialize called after invalid socket construction." 
         << " Address : " << this << endl;
    return;
  }

  sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
}


//TODO
void Blob::UdpSocket::Send(Packet* packet) {
  if (!packet) {
    cerr << "Null packet to send. Address : " << this << endl;
    return;
  }


}


//TODO
Blob::Packet* Blob::UdpSocket::Receive(void) {
  return nullptr;
}


