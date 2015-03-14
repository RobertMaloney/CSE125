#ifndef GV_ENDIANNESS_H
#define GV_ENDIANNESS_H

namespace Gv {

  class ByteOrder {

  public:

    static short HostToNet(short bytes);
    static int HostToNet(int bytes);
    static long HostToNet(long bytes);
    static float HostToNet(float bytes);
    static double HostToNet(double bytes);
    

    static short NetToHost(short bytes);
    static int NetToHost(int bytes);
    static long NetToHost(long bytes);
    static float NetToHost(float bytes);
    static double NetToHost(double bytes);

  private:
    
    static inline void NetToHost(char* first, unsigned int size);
    static inline void HostToNet(char* first, unsigned int size);
    static inline bool isBigEndian();

  };

}
#endif

/*

unsigned int res = 0;
unsigned int shiftamt = 0;
unsigned int lsbmask = 0xFF;
unsigned int msbmask = 0xFF << ((sizeof (int) - 1) << 3);
unsigned int orig = static_cast<unsigned int>(bytes);

for (int i = sizeof(int) >> 1; i > 0; --i) {
shiftamt = (i << 1) - 1;
res |= (orig & lsbmask) << (shiftamt << 3);
res |= (orig & msbmask) >> (shiftamt << 3);
lsbmask = lsbmask << 8;
msbmask = msbmask >> 8;
}
*/