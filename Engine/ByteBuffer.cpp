#include "ByteBuffer.h"


Blob::ByteBuffer::ByteBuffer(int capacity) {
  front = new char[capacity];
  size = 0;
  this->capacity = capacity;
}


Blob::ByteBuffer::~ByteBuffer(void) {
  if (front) {
    delete front;
  }

  front = nullptr;
  size = 0;
  capacity = 0;
}


void Blob::ByteBuffer::Expand(int necesarrySize) {
  int ncap = capacity;
  while (necesarrySize > ncap) {
    ncap <<= 1;
  }
  char* res = new char[ncap];
  memmove(static_cast<void*>(res), static_cast<void*>(front), size);
  delete front;
  front = res;
  capacity = ncap;
}


void Blob::ByteBuffer::Append(char* firstByte, unsigned int numBytes) {
  if (!firstByte) {
    return;
  }
  
  if (size + numBytes >= capacity) {
    this->Expand(size + numBytes);
  }

  memmove(static_cast<void*>(&front[size]), static_cast<void*>(firstByte), numBytes);
  size += numBytes;
}


void Blob::ByteBuffer::Append(char byte) {
  if (size == capacity) {
    this->Expand(size + 1);
  }
  front[size] = byte;
  size++;
}


void Blob::ByteBuffer::Append(short bytes) {
  this->Append((char*) &bytes, sizeof(bytes));
}


void Blob::ByteBuffer::Append(int bytes) {
  this->Append((char*) &bytes, sizeof(bytes));
}


void Blob::ByteBuffer::Append(long bytes) {
  this->Append((char*) &bytes, sizeof(bytes));
}


void Blob::ByteBuffer::Append(float bytes) {
  this->Append((char*) &bytes, sizeof(bytes));
}


void Blob::ByteBuffer::Append(double bytes) {
  this->Append((char*) &bytes, sizeof(bytes));
}


int Blob::ByteBuffer::Size(void) {
  return size;
}


int Blob::ByteBuffer::Capacity(void) {
  return capacity;
}


char* Blob::ByteBuffer::Front(void) {
  return front;
}


char* Blob::ByteBuffer::Last(void) {
  return &front[size - 1];
}


char* Blob::ByteBuffer::Back(void) {
  return front + (capacity - 1);
}


char Blob::ByteBuffer::operator[](unsigned int i) const {
  if (i >= capacity) {
    throw out_of_range(i + " is out of range.");
  }
  return front[i];
}


char & Blob::ByteBuffer::operator[](unsigned int i) {
  if (i >= capacity) {
    throw out_of_range(i + " is out of range.");
  }
  return front[i];
}
