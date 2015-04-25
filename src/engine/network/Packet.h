#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>

#include "Endianness.h"

using std::vector;

typedef uint8_t byte;

const int DEFAULT_PACKET_SIZE = 40;

class Packet {

public:

    Packet();
    Packet(int size);
    ~Packet();

    void writeByte(byte val);
    void writeBool(bool val);
    void writeChar(char val);
    void writeShort(short val);
    void writeInt(int val); 
    void writeLong(long val);
    void writeFloat(float val);
    void writeDouble(double val);

    void writeUChar(unsigned char val);
    void writeUShort(unsigned short val);
    void writeUInt(unsigned int val);
    void writeULong(unsigned long val);

    byte readByte();
    bool readBool();
    char readChar();
    short readShort();
    int readInt();
    long readLong();
    float readFloat();
    double readDouble();

    unsigned char readUChar();
    unsigned short readUShort();
    unsigned int readUInt();
    unsigned long readULong();

    void reset();
	void retreat(unsigned int num);
    void clear();
    unsigned int size() const;
    void resize(unsigned int size);
    void reserve(unsigned int cap);
	unsigned int getIndex();
	void setIndex(unsigned int nIndex);

    byte at(int index);
    byte& operator[](int i) {
        return buffer[i];
    }

private:


    inline void append(char* val, unsigned int size) { 
        if (size > 1 && !isBigEndian()) {
            byteSwap(val, size);
        }
        if (buffer.size() + size > buffer.capacity()) {
            buffer.reserve(buffer.capacity() * 2);
        }
        buffer.resize(buffer.size() + size);
        memcpy(reinterpret_cast<void*>(&buffer[buffer.size() - size]), reinterpret_cast<void*>(val), size);
    }


	inline void readRaw(char* buff, unsigned int size){
		if (size <= 0){
			return;
		}
		memcpy(reinterpret_cast<void*>(buff), reinterpret_cast<void*>(&buffer[index]), size);
		index += size;
	}

    inline void read(char* buff, unsigned int size) {
        if (size == 0) {
            return;
        }
        memcpy(reinterpret_cast<void*>(buff), reinterpret_cast<void*>(&buffer[index]), size);
        index += size;
        if (!isBigEndian()) {
            byteSwap(buff, size);
        }
    }


    unsigned int index;
    vector<byte> buffer;

    friend class TCPConnection;
    
};


#endif