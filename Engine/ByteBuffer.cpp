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


char Gv::ByteBuffer::operator[](int i) const {
  if (i < 0 || i >= capacity) {
    throw out_of_range(i + " is out of range.");
  }
  return front[i];
}


char & Gv::ByteBuffer::operator[](int i) {
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


// just here so it compiles
int main(int argc, char* arv[]) {

  return 0;
}