#include "Packet.h"

Packet::Packet() {
    buffer.reserve(DEFAULT_PACKET_SIZE);
    index = 0;
}

Packet::Packet(int size) {
    buffer.reserve(size);
    index = 0;
}


Packet::~Packet() {

}


void Packet::WriteByte(byte val) {
    this->buffer.push_back(val);
}


void Packet::WriteBool(bool val) {
    this->buffer.push_back(val);
}


void Packet::WriteChar(char val) {
    this->buffer.push_back(val);
}


void Packet::WriteUChar(unsigned char val) {
    this->buffer.push_back(val);
}


void Packet::WriteShort(short val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteUShort(unsigned short val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteInt(int val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteUInt(unsigned int val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteLong(long val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteULong(unsigned long val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteFloat(float val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteDouble(double val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


byte Packet::ReadByte() {
    return this->buffer[index++];
}


bool Packet::ReadBool() {
    return static_cast<bool>(this->buffer[index++]);
}


char Packet::ReadChar() {
    return this->buffer[index++];
}


unsigned char Packet::ReadUChar() {
    return this->buffer[index++];
}


short Packet::ReadShort() {
    short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(short));
    return buff;
}


unsigned short Packet::ReadUShort() {
    unsigned short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned short));
    return buff;
}


int Packet::ReadInt() {
    int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(int));
    return buff;
}


unsigned int Packet::ReadUInt() {
    unsigned int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned int));
    return buff;
}


long Packet::ReadLong() {
    long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(long));
    return buff;
}


unsigned long Packet::ReadULong() {
    unsigned long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned long));
    return buff;
}


float Packet::ReadFloat() {
    float buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(float));
    return buff;
}


double Packet::ReadDouble() {
    double buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(double));
    return buff;
}


void Packet::Reset() {
    index = 0;
}


void Packet::Clear() {
    buffer.clear();
    index = 0;
}


unsigned int Packet::Size() const {
    return buffer.size();
}


void Packet::Resize(unsigned int size) {
    buffer.resize(size);
}


void Packet::Reserve(unsigned int cap) {
    buffer.reserve(cap);
}


byte Packet::At(int index) {
    return buffer[index];
}