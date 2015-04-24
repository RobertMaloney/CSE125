#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <cstdint>
#include <vector>
#include <deque>

#include "Socket.h"
#include "Packet.h"

using std::vector;
using std::deque;

const int DEFAULT_SOCKET_BUFSIZ = 4096;             // the default size of buffers
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
    SocketError connect(const string & ip, const string & port);

    /* Send a single packet. Can return many errors. Check Socket.h SocketError
     * enum for relevant errors to the situation you are using it in.
     */
    SocketError send(const Packet & packet);

    /* Send all the packets in the vector. If one of the packets is larger than
     * MAX_PACKET_SIZE it will be silently dropped.
     */
    SocketError send(const deque<Packet> & packets);

    /* Receive a single packet into the packet buffer.
     */
    SocketError receive(Packet & packet);

    /* Get all the packets you can from the internal buffer. Use this if you need
     * to grab all packets instead of looping on the single receive. This will be
     * more efficient.
     */
    SocketError receive(deque<Packet> & packets);


protected:

    SocketError send();                   // internally used send and recvs'
    SocketError receive();
    uint32_t readHeader(const int start); // read the next packet size

    bool writeToBuffer(const Packet & packet); // write to the send buffer
    bool fillFromBuffer(Packet & packet, unsigned int & pos); // read from the receive buffer
    void shiftBuffer(vector<byte> & buffer, unsigned int lastRead); // shift from lastRead to size

    // prints all values in the specified buffer. useful for debugging.
    void printBuffer(vector<byte> & buffer, std::string msg);

    // vanilla send and receive wrappers
    inline int recv(byte* buffer, int size);
    inline int send(byte* buffer, int size);

    // internal buffers for sending and receiving
    vector<byte> sendBuffer;
    vector<byte> receiveBuffer;

};


inline int TCPConnection::send(byte* buffer, int size) {
#ifdef _WIN32
    return ::send(sock, reinterpret_cast<char*>(buffer), size, 0);
#else
    return ::send(sock, reinterpret_cast<void*>(buffer), size, 0);
#endif
}


int TCPConnection::recv(byte* buffer, int size) {
#ifdef _WIN32
    return ::recv(sock, reinterpret_cast<char*>(buffer), size, 0);
#else
    return ::recv(sock, reinterpret_cast<void*>(buffer), size, 0);
#endif
}


#endif