#include "ByteOrder.h"


inline void Gv::ByteOrder::byteSwap(char* first, unsigned int size) {
  char bmask = (char) 0xFF;
  char temp = 0;
  char* lsb = first;
  char* msb = &first[size - 1];

  for (; size > 0; size -= 2){
    temp = *lsb & bmask;
    *lsb = *msb & bmask;
    *msb = temp;
    lsb++; 
    msb--;
  }
}


inline bool Gv::ByteOrder::isBigEndian() {
  int x = 1;
  return ((*(char*)&x) == 0);
}


short Gv::ByteOrder::HostToNet(short bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


int Gv::ByteOrder::HostToNet(int bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


long Gv::ByteOrder::HostToNet(long bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


float Gv::ByteOrder::HostToNet(float bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


double Gv::ByteOrder::HostToNet(double bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


short Gv::ByteOrder::NetToHost(short bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


int Gv::ByteOrder::NetToHost(int bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


long Gv::ByteOrder::NetToHost(long bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


float Gv::ByteOrder::NetToHost(float bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


double Gv::ByteOrder::NetToHost(double bytes) {
  if (isBigEndian()) { return bytes; }
  byteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}
