#ifndef BLOB_BYTEBUFFER_H
#define BLOB_BYTEBUFFER_H

#include <stdexcept>

using std::out_of_range;

namespace Blob {

  class ByteBuffer {

  public:

    static const int DEFAULT_CAPACITY = 1500;

    ByteBuffer(int capacity = DEFAULT_CAPACITY);
    ~ByteBuffer(void);

    void Expand(int ncap);

    void Append(char* first, unsigned int size);
    void Append(char byte);
    void Append(short bytes);
    void Append(int bytes);
    void Append(long bytes);
    void Append(float bytes);
    void Append(double bytes);

    int Size(void);
    int Capacity(void);

    char* Front(void);
    char* LastElement(void);
    char* Back(void);

    char operator[](unsigned int i) const;
    char & operator[](unsigned int i);

  private:

    char* front;
    unsigned int size;
    unsigned int capacity;

  }; // End of ByteBuffer.h

}

#endif