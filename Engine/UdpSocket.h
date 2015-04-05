#ifndef BLOB_UDPSOCKET_H
#define BLOB_UDPSOCKET_H

#include "Socket.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

namespace Blob {

  class UdpSocket : public Socket {

  public:

    UdpSocket(SocketAddress addr);
    ~UdpSocket(void);

    void Initialize(void) override;
    void Send(Packet* packet) override;
    Packet* Receive(void) override;

  private:

    
  };

}
#endif