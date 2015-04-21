#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>

#include "Endianness.h"

using std::vector;

typedef uint8_t byte;

class Packet {

public:

    Packet(int size);
    ~Packet();

    void writeBool(bool val);
    void writeChar(char val);
    void writeUChar(unsigned char val);
    void writeShort(short val);
    void writeUShort(unsigned short val);
    void writeInt(int val);
    void writeUInt(unsigned int val);
    void writeLong(long val);
    void writeULong(unsigned long val);
    void writeFloat(float val);
    void writeDouble(double val);

    bool readBool();
    char readChar();
    unsigned char readUChar();
    short readShort();
    unsigned short readUShort();
    int readInt();
    unsigned int readUInt();
    long readLong();
    unsigned long readULong();
    float readFloat();
    double readDouble();

    byte* data();
    void reset();
    unsigned int size();
    unsigned int currentIndex();
    void resize(unsigned int size);
    void reserve(unsigned int cap);
    vector<byte>::iterator begin();
    vector<byte>::iterator end();

    byte& operator[](int i) {
        return buffer[i];
    }

private:


    inline void append(char* val, unsigned int size) {         
        if (size > 1 && !IsBigEndian()) {
            ByteSwap(val, size);
        }
        buffer.resize(buffer.size() + size);
        memcpy(reinterpret_cast<void*>(&(buffer[buffer.size() - size])), reinterpret_cast<void*>(val), size);
    }


    inline void read(char* buff, unsigned int size) {
        if (size == 0) {
            return;
        }
        memcpy(reinterpret_cast<void*>(buff), reinterpret_cast<void*>(&buffer[index]), size);
        if (!IsBigEndian()) {
            ByteSwap(buff, size);
        }
    }


    unsigned int index;
    vector<byte> buffer;

};


#endif