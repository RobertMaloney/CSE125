#ifndef BLOB_ENDIANNESS_H
#define BLOB_ENDIANNESS_H


inline short HostToNet(short bytes);
inline int HostToNet(int bytes);
inline long HostToNet(long bytes);
inline float HostToNet(float bytes);
inline double HostToNet(double bytes);

inline short NetToHost(short bytes);
inline int NetToHost(int bytes);
inline long NetToHost(long bytes);
inline float NetToHost(float bytes);
inline double NetToHost(double bytes);

inline void ByteSwap(char* first, unsigned int size);
inline bool IsBigEndian(void);


#include "Endianness.inl"
#endif


