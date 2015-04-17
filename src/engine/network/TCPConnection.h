#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstdint>
#include <vector>
#include <deque>

#include "Socket.h"

using std::vector;
using std::deque;

typedef uint8_t byte;
typedef vector<byte> Packet;

const int DEFAULT_SOCKET_BUFSIZ = 8192;
const int BYTES_IN_HEADER = 4;
const int FREE_THRESHOLD = 512;
const int MAX_SOCKET_BUFSIZ = 65511;

class TCPConnection : public Socket {

public:

    TCPConnection();
    TCPConnection(int sockFd, SocketAddress remAddr);
    ~TCPConnection();

    SocketError Connect(const string & ip, const string & port);

    SocketError Send(const Packet & packet);
    SocketError Receive(Packet & packet);


protected:

    int Send(const void* data, int size);
    int Receive(void* buffer, int buffSize);
    bool FillFromBuffer(Packet & packet);
    void ExpandReceiveBuff();

	int nextPacketSize;

    vector<byte> sendBuffer;
    vector<byte> receiveBuffer;
};

#endif