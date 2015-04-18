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

const int DEFAULT_SOCKET_BUFSIZ = 10;
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

    int Send(const void* data, int size);
    int Receive(void* buffer, int buffSize);
	void WriteHeader(uint32_t size, vector<byte> buffer);
	void GetAllFromBuffer(vector<Packet> & packets);
    bool GetFromBuffer(Packet & packet);
    void ExpandReceiveBuff();

	uint32_t nextPacketSize;

    vector<byte> sendBuffer;
    vector<byte> receiveBuffer;
};

#endif