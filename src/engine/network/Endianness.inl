


inline void ByteSwap(char* first, unsigned int size) {
    if (size < 2) {
        return;
    }

    char bmask = (char) 0xFF;              
    char temp = 0;
    char* laddr = first;              // the lowest address of bytes in the value
    char* haddr = &first[size - 1];   // the highest address of bytes in the value

    // swap each byte 
    for (; size > 0; size -= 2) {
        temp = *laddr & bmask;
        *laddr = *haddr & bmask;
        *haddr = temp;
        laddr++;
        haddr--;
    }
}


inline bool IsBigEndian() {
    int x = 1;
    return ((*(char*) &x) == 0);
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

inline size_t HostToNet(size_t bytes) {
	if (IsBigEndian()) {
		return bytes;
	}
	ByteSwap((char*)&bytes, sizeof(bytes));
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

inline size_t NetToHost(size_t bytes) {
	if (IsBigEndian()) {
		return bytes;
	}
	ByteSwap((char*)&bytes, sizeof(bytes));
	return bytes;
}