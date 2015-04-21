#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <vector>
#include <cstdint>


#include "Endianness.h"

using std::cout;
using std::vector;

typedef uint8_t byte;

const int DEFAULT_PACKET_SIZE = 40;

class Packet {

public:

    Packet();
    Packet(int size);
    ~Packet();

    void WriteByte(byte val);
    void WriteBool(bool val);
    void WriteChar(char val);
    void WriteShort(short val);
    void WriteInt(int val); 
    void WriteLong(long val);
    void WriteFloat(float val);
    void WriteDouble(double val);

    void WriteUChar(unsigned char val);
    void WriteUShort(unsigned short val);
    void WriteUInt(unsigned int val);
    void WriteULong(unsigned long val);

    byte ReadByte();
    bool ReadBool();
    char ReadChar();
    short ReadShort();
    int ReadInt();
    long ReadLong();
    float ReadFloat();
    double ReadDouble();

    unsigned char ReadUChar();
    unsigned short ReadUShort();
    unsigned int ReadUInt();
    unsigned long ReadULong();
;

    void Reset();
    void Clear();
    unsigned int Size() const;
    void Resize(unsigned int size);
    void Reserve(unsigned int cap);
    
    byte At(int index);
    byte& operator[](int i) {
        return buffer[i];
    }

private:


    inline void append(char* val, unsigned int size) { 
        if (size > 1 && !IsBigEndian()) {
            ByteSwap(val, size);
        }
        if (buffer.size() + size > buffer.capacity()) {
            buffer.reserve(buffer.capacity() * 2);
        }
        buffer.resize(buffer.size() + size);
        memcpy(reinterpret_cast<void*>(&buffer[buffer.size() - size]), reinterpret_cast<void*>(val), size);
    }


    inline void read(char* buff, unsigned int size) {
        if (size == 0) {
            return;
        }
        memcpy(reinterpret_cast<void*>(buff), reinterpret_cast<void*>(&buffer[index]), size);
        index += size;
        if (!IsBigEndian()) {
            ByteSwap(buff, size);
        }
    }


    unsigned int index;
    vector<byte> buffer;

    friend class TCPConnection;
    
};


#endif