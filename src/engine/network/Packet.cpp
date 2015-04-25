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


void Packet::writeByte(byte val) {
    this->buffer.push_back(val);
}


void Packet::writeBool(bool val) {
    this->buffer.push_back(val);
}


void Packet::writeChar(char val) {
    this->buffer.push_back(val);
}


void Packet::writeUChar(unsigned char val) {
    this->buffer.push_back(val);
}


void Packet::writeShort(short val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeUShort(unsigned short val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeInt(int val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeUInt(unsigned int val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeLong(long val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeULong(unsigned long val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeFloat(float val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeDouble(double val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


byte Packet::readByte() {
    return this->buffer[index++];
}


bool Packet::readBool() {
    return (this->buffer[index++]) ? true : false;
}


char Packet::readChar() {
    return this->buffer[index++];
}


unsigned char Packet::readUChar() {
    return this->buffer[index++];
}


short Packet::readShort() {
    short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(short));
    return buff;
}


unsigned short Packet::readUShort() {
    unsigned short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned short));
    return buff;
}


int Packet::readInt() {
    int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(int));
    return buff;
}


unsigned int Packet::readUInt() {
    unsigned int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned int));
    return buff;
}


long Packet::readLong() {
    long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(long));
    return buff;
}


unsigned long Packet::readULong() {
    unsigned long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(unsigned long));
    return buff;
}


float Packet::readFloat() {
    float buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(float));
    return buff;
}


double Packet::readDouble() {
    double buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(double));
    return buff;
}


void Packet::reset() {
    index = 0;
}


void Packet::retreat(unsigned int i) {
	this->index -= i;
}

void Packet::clear() {
    buffer.clear();
    index = 0;
}


unsigned int Packet::size() const {
    return buffer.size();
}


void Packet::resize(unsigned int size) {
    buffer.resize(size);
}


void Packet::reserve(unsigned int cap) {
    buffer.reserve(cap);
}


byte Packet::at(int index) {
    return buffer[index];
}