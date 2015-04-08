#ifndef BLOB_UDPSOCKET_H
#define BLOB_UDPSOCKET_H

#include "Socket.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

namespace Blob {

  // TODO
  class UdpSocket : public Socket {

  public:

    UdpSocket(SocketAddress addr);
    ~UdpSocket();

    void Initialize() override;
    void Send(Packet* packet) override;
    Packet* Receive() override;

  private:

    
  };

}
#endif
