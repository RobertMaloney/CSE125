#include "Packet.h"


Packet::Packet(int size) {
    buffer.reserve(size);
    index = 0;
}


Packet::~Packet() {

}


void Packet::writeBool(bool val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeChar(char val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
}


void Packet::writeUChar(unsigned char val) {
    this->append(reinterpret_cast<char*>(&val), sizeof(val));
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


bool Packet::readBool() {
    bool buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


char Packet::readChar() {
    char buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned char Packet::readUChar() {
    unsigned char buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


short Packet::readShort() {
    short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned short Packet::readUShort() {
    unsigned short buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


int Packet::readInt() {
    int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned int Packet::readUInt() {
    unsigned int buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


long Packet::readLong() {
    long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


unsigned long Packet::readULong() {
    unsigned long buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


float Packet::readFloat() {
    float buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


double Packet::readDouble() {
    double buff;
    this->read(reinterpret_cast<char*>(&buff), sizeof(bool));
    return buff;
}


byte* Packet::data() {
    return buffer.data();
}


void Packet::reset() {
    index = 0;
}


unsigned int Packet::size() {
    return buffer.size();
}


unsigned int Packet::currentIndex() {
    return index;
}


void Packet::resize(unsigned int size) {
    buffer.resize(size);
}


void Packet::reserve(unsigned int cap) {
    buffer.reserve(cap);
}


vector<byte>::iterator Packet::begin() {
    return buffer.begin();
}


vector<byte>::iterator Packet::end() {
    return buffer.end();
}
