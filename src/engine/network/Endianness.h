#ifndef ENDIANNESS_H
#define ENDIANNESS_H

/* These methods all reverse the byte ordering of multi-byte
 * values. They should be used whenever trying to send a multi-byte
 * value over a socket. 
 */

inline short hostToNet(short bytes);
inline int hostToNet(int bytes);
inline long hostToNet(long bytes);
inline float hostToNet(float bytes);
inline double hostToNet(double bytes);
inline size_t hostToNet(size_t bytes);

inline short netToHost(short bytes);
inline int netToHost(int bytes);
inline long netToHost(long bytes);
inline float netToHost(float bytes);
inline double netToHost(double bytes);
inline size_t netToHost(size_t bytes);

inline void byteSwap(char* first, unsigned int size);
inline bool isBigEndian();          // tells if the current system is big endian


#include "Endianness.inl"
#endif


