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

    void Reset();
    unsigned int Index();
    unsigned int Size();
    byte* Data();

    byte& operator[](int i) {
        return data[i];
    }

private:


    inline void append(char* val, unsigned int size) {         
        if (size > 1 && !IsBigEndian()) {
            ByteSwap(val, size);
        }
        data.resize(data.size() + size);
        memcpy(reinterpret_cast<void*>(&(data[data.size() - size])), reinterpret_cast<void*>(val), size);
    }


    inline void read(char* buff, unsigned int size) {
        if (size == 0) {
            return;
        }
        memcpy(reinterpret_cast<void*>(buff), reinterpret_cast<void*>(&data[index]), size);
        if (!IsBigEndian()) {
            ByteSwap(buff, size);
        }
    }


    unsigned int index;
    vector<byte> data;

};


#endif