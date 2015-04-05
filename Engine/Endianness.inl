


inline void ByteSwap(char* first, unsigned int size) {
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


inline bool IsBigEndian(void) {
  int x = 1;
  return ((*(char*)&x) == 0);
}


inline short HostToNet(short bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline int HostToNet(int bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline long HostToNet(long bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline float HostToNet(float bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline double HostToNet(double bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline short NetToHost(short bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline int NetToHost(int bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline long NetToHost(long bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline float NetToHost(float bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}


inline double NetToHost(double bytes) {
  if (IsBigEndian()) { return bytes; }
  ByteSwap((char*) &bytes, sizeof(bytes));
  return bytes;
}
