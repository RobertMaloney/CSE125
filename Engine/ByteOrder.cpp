#include "ByteOrder.h"

#include <iostream>
inline bool Gv::ByteOrder::isBigEndian() {
  int x = 1;
  return !((*(char*)&x) == 1);
}

void printBits(short byte) {

};
short Gv::ByteOrder::HostToNet(short bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


void Gv::ByteOrder::HostToNet(char* first, unsigned int size) {
  if (isBigEndian()) {
    return;
  }

  char bmask = (char)(0xFF);
  char temp = 0;
  char* lsb = first;
  char* msb = &first[size - 1];

  while (lsb < msb) {
    temp = *lsb & bmask;
    *lsb &= 0;
    *lsb |= *msb & bmask;
    *msb &= 0;
    *msb |= temp;
    lsb++;
    msb--;
  }
}


void Gv::ByteOrder::NetToHost(char* first, unsigned int size) {

}


int Gv::ByteOrder::HostToNet(int bytes) {
  if (isBigEndian()) {
    return bytes;
  }


  return 0;
}


long Gv::ByteOrder::HostToNet(long bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


float Gv::ByteOrder::HostToNet(float bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


double Gv::ByteOrder::HostToNet(double bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


short Gv::ByteOrder::NetToHost(short bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


int Gv::ByteOrder::NetToHost(int bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


long Gv::ByteOrder::NetToHost(long bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


float Gv::ByteOrder::NetToHost(float bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}


double Gv::ByteOrder::NetToHost(double bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  return 0;
}
