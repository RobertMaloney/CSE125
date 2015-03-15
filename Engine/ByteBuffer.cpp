#include "ByteBuffer.h"


Gv::ByteBuffer::ByteBuffer(int capacity) {
  front = new char[capacity];
  size = 0;
  this->capacity = capacity;
}


Gv::ByteBuffer::~ByteBuffer() {
  if (front) {
    delete front;
  }

  front = nullptr;
  size = 0;
  capacity = 0;
}


void Gv::ByteBuffer::Expand(int ncap) {
  char* res = new char[ncap];
  memmove(static_cast<void*>(res), static_cast<void*>(front), size);
  delete front;
  front = res;
  capacity = ncap;
}


void Gv::ByteBuffer::Append(char* firstByte, unsigned int numBytes) {
  if (!firstByte) {
    return;
  }
  
  while (size + numBytes >= capacity) {
    this->Expand(capacity << 1);
  }

  memmove(static_cast<void*>(&front[size]), 
    static_cast<void*>(firstByte), numBytes);
}


void Gv::ByteBuffer::Append(char byte) {
  if (size == capacity) {
    this->Expand(capacity << 1);
  }
  front[size] = byte;
  size++;
}


void Gv::ByteBuffer::Append(short bytes) {
  while (size + sizeof(short) >= capacity) {
    this->Expand(capacity << 1);
  }
  this->Append((char*) &bytes, sizeof(bytes));
}


void Gv::ByteBuffer::Append(int bytes) {
  while (size + sizeof(int) >= capacity) {
    this->Expand(capacity << 1);
  }
  this->Append((char*) &bytes, sizeof(bytes));
}


void Gv::ByteBuffer::Append(long bytes) {
  while (size + sizeof(long) >= capacity) {
    this->Expand(capacity << 1);
  }
  this->Append((char*) &bytes, sizeof(bytes));
}


void Gv::ByteBuffer::Append(float bytes) {
  while (size + sizeof(float) >= capacity) {
    this->Expand(capacity << 1);
  }
  this->Append((char*) &bytes, sizeof(bytes));
}


void Gv::ByteBuffer::Append(double bytes) {
  while (size + sizeof(double) >= capacity) {
    this->Expand(capacity << 1);
  }
  this->Append((char*) &bytes, sizeof(bytes));
}


int Gv::ByteBuffer::Size() {
  return size;
}


int Gv::ByteBuffer::Capacity() {
  return capacity;
}


char* Gv::ByteBuffer::Front() {
  return front;
}


char* Gv::ByteBuffer::Last() {
  return &front[size - 1];
}


char* Gv::ByteBuffer::Back() {
  return front + (capacity - 1);
}


char Gv::ByteBuffer::operator[](unsigned int i) const {
  if (i >= capacity) {
    throw out_of_range(i + " is out of range.");
  }
  return front[i];
}


char & Gv::ByteBuffer::operator[](unsigned int i) {
  if (i < 0 || i >= capacity) {
    throw out_of_range(i + " is out of range.");
  }
  return front[i];
}


#include <iostream> 
#include "ByteOrder.h"

// just here so it compiles
int main(int argc, char* arv[]) {
  short y = 128;
  std::cout << "orig " << y << " " <<  Gv::ByteOrder::HostToNet(y) << std::endl;
  return 0;
}