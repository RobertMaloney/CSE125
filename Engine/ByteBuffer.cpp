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


void Gv::ByteBuffer::Append(char byte) {
  if (size == capacity) {
    Expand(capacity << 1);
  }
  front[size] = byte;
  size++;
}


void Gv::ByteBuffer::Append(short bytes) {
  if (size + sizeof(short) >= capacity) {
    Expand(capacity << 1);
  }

}


void Gv::ByteBuffer::Append(int bytes) {
  if (size + sizeof(int) >= capacity) {
    Expand(capacity << 1);
  }

}


void Gv::ByteBuffer::Append(long bytes) {
  if (size + sizeof(long) >= capacity) {
    Expand(capacity << 1);
  }

}


void Gv::ByteBuffer::Append(float bytes) {
  if (size + sizeof(float) >= capacity) {
    Expand(capacity << 1);
  }

}


void Gv::ByteBuffer::Append(double bytes) {
  if (size + sizeof(double) >= capacity) {
    Expand(capacity << 1);
  }

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


void Gv::ByteBuffer::Expand(int ncap) {
  char* res = new char[ncap];
  memmove(static_cast<void*>(res), static_cast<void*>(front), size);
  delete front;
  front = res;
  capacity = ncap;
}

#include <iostream>
#include "ByteOrder.h"

// just here so it compiles
int main(int argc, char* arv[]) {

  short x = 128;
  int y = 128;
  char c;
  std::cout << "orig " << y;
  Gv::ByteOrder::HostToNet((char*)&y, sizeof(int));

  std::cout << " swapped " << y << std::endl;
//  std::cout << "orig " << y << " swapped " << Gv::ByteOrder::HostToNet(y) << std::endl;
  std::cin >> c;
  return 0;
}