#include "Packet.h"


Packet::Packet(int size) {
    data.reserve(size);
    index = 0;
}


Packet::~Packet() {

}


void Packet::WriteBool(bool val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteChar(char val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::WriteUChar(unsigned char val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
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


bool Packet::ReadBool() {
    bool buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


char Packet::ReadChar() {
    char buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned char Packet::ReadUChar() {
    unsigned char buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


short Packet::ReadShort() {
    short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned short Packet::ReadUShort() {
    unsigned short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


int Packet::ReadInt() {
    int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned int Packet::ReadUInt() {
    unsigned int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


long Packet::ReadLong() {
    long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned long Packet::ReadULong() {
    unsigned long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


float Packet::ReadFloat() {
    float buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


double Packet::ReadDouble() {
    double buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


void Packet::Reset() {
    index = 0;
}


unsigned int Packet::Index() {
    return index;
}


unsigned int Packet::Size() {
    return data.size();
}


byte* Packet::Data() {
    return data.data();
}