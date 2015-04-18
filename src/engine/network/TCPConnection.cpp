#include "TCPConnection.h"


TCPConnection::TCPConnection() : Socket() {
    this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
	sendBuffer.resize(0);
	receiveBuffer.resize(0);
}


TCPConnection::TCPConnection(int sockFd, SocketAddress remAddr) : Socket() {
    this->sock = sockFd;
    this->remoteAddress = remAddr;
    this->nextPacketSize = 0;
    sendBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
    receiveBuffer.reserve(DEFAULT_SOCKET_BUFSIZ);
	sendBuffer.resize(0);
	receiveBuffer.resize(0);
}


TCPConnection::~TCPConnection() {
    this->Close();
}

SocketError TCPConnection::Connect(const string & ip, const string & port) {
    AddressInfo *iter = this->DNSLookup(ip, port, SOCK_STREAM);

    for (; iter; iter = iter->ai_next) {
        sock = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (sock == INACTIVE_SOCKET) {
            continue;
        }
        if (::connect(sock, iter->ai_addr, iter->ai_addrlen) != 0) {
            this->Close(sock);
            continue;
        }
        break;
    }
    if (iter != nullptr) {
        freeaddrinfo(iter);
    }
    return (sock == INACTIVE_SOCKET) ? SE_NOCONNECT : SE_NOERR;
}

void printBuffer(vector<byte> & buffer) {
	int oldSize = buffer.size();
	buffer.resize(buffer.capacity());
	for (int i = 0; i < buffer.size(); ++i){
		std::cout << std::to_string(buffer[i]);
	}
	std::cout << "\n";
	buffer.resize(oldSize);
}
SocketError TCPConnection::Send(const Packet & packet) {
    // if theres nothing to send do nothing
    if (packet.size() == 0) {
        return SE_NOERR;
    }

    // if there's a problem with the packet's size return the PACKETSIZE error
    if (packet.size() > MAX_PACKET_SIZE || packet.size() < 0) {
        return SE_PACKETSIZE;
    }
  
   /* // reserve enough space in the buffer to store the entire packet.
    if (packet.size() > sendBuffer.capacity() - sendBuffer.size()) {
        sendBuffer.reserve(sendBuffer.size() + packet.size() + sizeof(packet.size()));
    }*/

    // write the packet size to the buffer
    uint32_t mask = 0xFF000000;
	uint32_t size = static_cast<uint32_t>(packet.size()); //HostToNet(static_cast<uint32_t>(packet.size()));
    for (int i = 3; i >= 0; --i) {
        sendBuffer.push_back((size & mask) >> (i * 8));
        mask >>= 8;
    }
	std::cout << "\nsize() : " << sendBuffer.size() << std::endl;
	std::cout << "size header only : ";
	printBuffer(sendBuffer);

    // put the body of the packet in the buffer
    for (auto it = packet.begin(); it != packet.end(); ++it) {
        sendBuffer.push_back(*it);
    }
	std::cout << "size & data : ";
	printBuffer(sendBuffer);
    // send as much as the OS will send
    int numSent = this->Send(sendBuffer.data(), sendBuffer.size());
	std::cout << "size and numsent : " << numSent << " ";
	printBuffer(sendBuffer);
    if (numSent == 0) {
        return SE_DISCONNECTED;
    } else if (numSent < 0) {     // if less than 0 there was some error so return it
        return this->GetError();
    }
    
	// move all the unsent (leftover) bytes to the front of the buffer
	unsigned int vpos, spos;
    for (vpos = numSent, spos = 0; vpos < sendBuffer.size(); ++spos, ++vpos) {
        sendBuffer[spos] = sendBuffer[vpos];
    }

    // change the size to be correct after the send
    sendBuffer.resize(sendBuffer.size() - numSent);
	std::cout << "buffer after shift : ";
	printBuffer(sendBuffer);
    return SE_NOERR;
}



SocketError TCPConnection::Receive(Packet & packet) {
	
	
	// Try to grab a packet from the buffer before calling receive.
  /*  if (this->FillFromBuffer(packet)) {
		std::cout << "Filled buffer" << std::endl;
        return SE_NOERR;
    }*/
	
    // if the buffer is full and we havent gotten a packet the packet's too large.
    if (receiveBuffer.size() > MAX_PACKET_SIZE) {
		std::cout << "Packet size" << std::endl;
        return SE_PACKETSIZE;
    }

    // Caclulate how much buffer space we have. If the buffer is small try to make it bigger
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();
   /* if (bytesAvail < FREE_THRESHOLD && receiveBuffer.capacity() < MAX_SOCKET_BUFSIZ) {
        this->ExpandReceiveBuff();
    }*/

    uint32_t buffPosition = receiveBuffer.size();
    receiveBuffer.resize(receiveBuffer.capacity());
    int bytesRecvd = this->Receive(receiveBuffer.data() + buffPosition, bytesAvail);

    // check for errors
    if (bytesRecvd == 0) {          // 0 means the socket isnt connected anymore
			receiveBuffer.resize(buffPosition);
        return SE_DISCONNECTED;
	} else if (bytesRecvd < 0) {    // < 0 means there was some error
		receiveBuffer.resize(buffPosition);
		return this->GetError();
	}
	receiveBuffer.resize(buffPosition + bytesRecvd);
    this->FillFromBuffer(packet);               // try to get a packet from the buffer
    return SE_NOERR;
}



int TCPConnection::Send(const void* data, int size) {
#ifdef _WIN32
    return ::send(sock, static_cast<const char*>(data), size, 0);
#else
    return ::send(sock, data, size, 0);
#endif
}


int TCPConnection::Receive(void* buffer, int buffSize) {
#ifdef _WIN32
    return ::recv(sock, static_cast<char*>(buffer), buffSize, 0);
#else
    return ::recv(sock, buffer, buffSize, 0);
#endif
}


bool TCPConnection::FillFromBuffer(Packet & packet) {
    // if the buffer is as long as the header read the header so we know how many bytes to grab
    if (receiveBuffer.size() < BYTES_IN_HEADER) {
        return false;
    }

    // If the buffer doesn't contain a complete packet then return false
    if (receiveBuffer.size() - BYTES_IN_HEADER < nextPacketSize) {
        return false;
    }
	printBuffer(receiveBuffer);
	nextPacketSize = receiveBuffer[0] << 24 | receiveBuffer[1] << 16 | receiveBuffer[2] << 8 | receiveBuffer[3];
	//std::cout << "receive buffer size : " << receiveBuffer.size() << " nextpacketsize : " << nextPacketSize << std::endl;
    // Copy a packet into the packet buffer we were passed
    for (int i = 0; i < nextPacketSize; ++i) {
        packet.push_back(receiveBuffer[i + BYTES_IN_HEADER]);
    }

    // calculate the leftover bytes then move them to the front of the buffer. THis way the nextPacketSize is always the 
    // first thing in the buffer. Resize the buffer so the size member variable is accurate.
    int bytesRemaining = receiveBuffer.size() - nextPacketSize - BYTES_IN_HEADER;
    for (int i = 0; i < bytesRemaining; ++i) {
		printBuffer(receiveBuffer);
        receiveBuffer[i] = receiveBuffer[i + BYTES_IN_HEADER + nextPacketSize];
    }

    receiveBuffer.resize(bytesRemaining);
	printBuffer(receiveBuffer);
    return true;
}


void TCPConnection::ExpandReceiveBuff() {
    int newSize = DEFAULT_SOCKET_BUFSIZ;
    int bytesAvail = receiveBuffer.capacity() - receiveBuffer.size();

    while (bytesAvail < FREE_THRESHOLD && receiveBuffer.capacity() < MAX_SOCKET_BUFSIZ) {
        newSize <<= 1;
        bytesAvail = newSize - receiveBuffer.size();
    }

    receiveBuffer.reserve(newSize);
}
