#ifndef GV_BYTEBUFFER_H
#define GV_BYTEBUFFER_H

#include <stdexcept>

using std::out_of_range;

namespace Gv {

  class ByteBuffer {

  public:

    static const int DEFAULT_CAPACITY = 1024;

    ByteBuffer(int capacity = DEFAULT_CAPACITY);
    ~ByteBuffer();

    void Expand(int newCapacity);

    void Append(char byte);
    void Append(short bytes);
    void Append(int bytes);
    void Append(long bytes);
    void Append(float bytes);
    void Append(double bytes);

    int Size();
    int Capacity();

    char* Front();
    char* Last();
    char* Back();

    char operator[](int i) const;
    char & operator[](int i);

  private:

    void append();

    char* front;
    int size;
    int capacity;

  }; // End of ByteBuffer.h

}

#endif