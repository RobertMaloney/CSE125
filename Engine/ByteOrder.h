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

    static inline bool isBigEndian();

  };

}
#endif