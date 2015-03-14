#include "ByteOrder.h"


inline bool Gv::ByteOrder::isBigEndian() {
  int x = 1;
  return (*(char*)&x) == 0;
}


short Gv::ByteOrder::HostToNet(short bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


int Gv::ByteOrder::HostToNet(int bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


long Gv::ByteOrder::HostToNet(long bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


float Gv::ByteOrder::HostToNet(float bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


double Gv::ByteOrder::HostToNet(double bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


short Gv::ByteOrder::NetToHost(short bytes) {
  if (isBigEndian()) {
    return bytes;
  }
 
}


int Gv::ByteOrder::NetToHost(int bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


long Gv::ByteOrder::NetToHost(long bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


float Gv::ByteOrder::NetToHost(float bytes) {
  if (isBigEndian()) {
    return bytes;
  }

}


double Gv::ByteOrder::NetToHost(double bytes) {
  if (isBigEndian()) {
    return bytes;
  }
  
}
