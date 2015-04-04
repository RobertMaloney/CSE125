#ifndef GV_UDPSOCKET_H
#define GV_UDPSOCKET_H

#include "Socket.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

namespace Gv {

  class UdpSocket : Socket {

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