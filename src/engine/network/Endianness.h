#ifndef ENDIANNESS_H
#define ENDIANNESS_H

/* These methods all reverse the byte ordering of multi-byte
 * values. They should be used whenever trying to send a multi-byte
 * value over a socket. 
 */

inline short HostToNet(short bytes);
inline int HostToNet(int bytes);
inline long HostToNet(long bytes);
inline float HostToNet(float bytes);
inline double HostToNet(double bytes);
inline size_t HostToNet(size_t bytes);

inline short NetToHost(short bytes);
inline int NetToHost(int bytes);
inline long NetToHost(long bytes);
inline float NetToHost(float bytes);
inline double NetToHost(double bytes);
inline size_t NetToHost(size_t bytes);

inline void ByteSwap(char* first, unsigned int size);
inline bool IsBigEndian();          // tells if the current system is big endian


#include "Endianness.inl"
#endif


