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

    void WriteByte(byte val);
    void WriteBool(bool val);
    void WriteChar(char val);
    void WriteUChar(unsigned char val);
    void WriteShort(short val);
    void WriteUShort(unsigned short val);
    void WriteInt(int val);
    void WriteUInt(unsigned int val);
    void WriteLong(long val);
    void WriteULong(unsigned long val);
    void WriteFloat(float val);
    void WriteDouble(double val);

    byte ReadByte();
    bool ReadBool();
    char ReadChar();
    unsigned char ReadUChar();
    short ReadShort();
    unsigned short ReadUShort();
    int ReadInt();
    unsigned int ReadUInt();
    long ReadLong();
    unsigned long ReadULong();
    float ReadFloat();
    double ReadDouble();

    byte* Data();
    void Reset();
    unsigned int Size();
    unsigned int Index();
    void Resize(unsigned int size);
    void Reserve(unsigned int cap);
    vector<byte>::iterator Begin();
    vector<byte>::iterator End();

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