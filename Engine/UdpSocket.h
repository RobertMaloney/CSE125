#ifndef GV_UDPSOCKET_H
#define GV_UDPSOCKET_H

#include "Socket.h"

namespace Gv {

  class UdpSocket : Socket {

  public:

    UdpSocket();
    ~UdpSocket();

    void Initialize() override;
    void Send(Packet* packet) override;
    Packet* Receive() override;

  private:

  };

}
#endif