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

const int DEFAULT_SOCKET_BUFSIZ = 1;             // the default size of buffers
const int BYTES_IN_HEADER = 4;                   
const int FREE_THRESHOLD = 512;                  // min number of avail bytes in buffer before expansion
const int MAX_SOCKET_BUFSIZ = MAX_PACKET_SIZE;

class TCPConnection : public Socket {

public:

    TCPConnection();
    TCPConnection(int sockFd, SocketAddress remAddr);
    ~TCPConnection();

    /* Connect to the specified ip and port. If the socket can not connect
     * an SE_NOCONNECT error will be returned
     */
    SocketError Connect(const string & ip, const string & port);

    /* Send a single packet. Can return many errors. Check Socket.h SocketError
     * enum for relevant errors to the situation you are using it in.
     */
    SocketError Send(const Packet & packet);

    /* Send all the packets in the vector. If one of the packets is larger than
     * MAX_PACKET_SIZE it will be silently dropped.
     */
    SocketError Send(const deque<Packet> & packets);

    /* Receive a single packet into the packet buffer.
     */
    SocketError Receive(Packet & packet);

    /* Get all the packets you can from the internal buffer. Use this if you need
     * to grab all packets instead of looping on the single receive. This will be
     * more efficient.
     */
    SocketError Receive(deque<Packet> & packets);


protected:

    SocketError Send();                   // internally used send and recvs'
    SocketError Receive();
    uint32_t ReadHeader(const int start); // read the next packet size

    bool WriteToBuffer(const Packet & packet); // write to the send buffer
    bool FillFromBuffer(Packet & packet, unsigned int & pos); // read from the receive buffer
    void ShiftBuffer(vector<byte> & buffer, unsigned int lastRead); // shift from lastRead to size 

    // vanilla send and receive wrappers
    inline int Send(byte* buffer, int size);
    inline int Recv(byte* buffer, int size);

    // prints all values in the specified buffer. useful for debugging.
    void PrintBuffer(vector<byte> & buffer, std::string msg);

    // internal buffers for sending and receiving
    vector<byte> sendBuffer;
    vector<byte> receiveBuffer;

};

#endif