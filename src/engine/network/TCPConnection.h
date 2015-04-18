#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstdint>
#include <vector>
#include <deque>

#include "Socket.h"

using std::vector;
using std::fill;

typedef uint8_t byte;
typedef vector<byte> Packet;

const int DEFAULT_SOCKET_BUFSIZ = 7;
const int BYTES_IN_HEADER = 4;
const int FREE_THRESHOLD = 512;
const int MAX_SOCKET_BUFSIZ = MAX_PACKET_SIZE;

class TCPConnection : public Socket {

public:

    TCPConnection();
    TCPConnection(int sockFd, SocketAddress remAddr);
    ~TCPConnection();

    SocketError Connect(const string & ip, const string & port);

    SocketError Send(const Packet & packet);
    SocketError Send(const vector<Packet> & packets);

    SocketError Receive(Packet & packet);
    SocketError Receive(vector<Packet> & packets);


protected:

    SocketError Send();
    SocketError Receive();
    uint32_t ReadHeader(const int start);

    bool WriteToBuffer(const Packet & packet);
    bool FillFromBuffer(Packet & packet, unsigned int & pos);
    void ShiftBuffer(vector<byte> & buffer, unsigned int lastRead);

    inline int Send(byte* buffer, int size);
    inline int Recv(byte* buffer, int size);

    void PrintBuffer(vector<byte> & buffer, std::string msg);

    vector<byte> sendBuffer;
    vector<byte> receiveBuffer;
};

#endif