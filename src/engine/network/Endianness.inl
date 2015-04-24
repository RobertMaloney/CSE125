


inline void byteSwap(char* first, unsigned int size) {
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


inline bool isBigEndian() {
    int x = 1;
    return ((*reinterpret_cast<char*>(&x)) == 0);
}


inline short hostToNet(short bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline int hostToNet(int bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline long hostToNet(long bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline float hostToNet(float bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline double hostToNet(double bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline short netToHost(short bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}

inline size_t hostToNet(size_t bytes) {
	if (isBigEndian()) {
		return bytes;
	}
	byteSwap((char*)&bytes, sizeof(bytes));
	return bytes;
}


inline int netToHost(int bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline long netToHost(long bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline float netToHost(float bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}


inline double netToHost(double bytes) {
    if (isBigEndian()) { return bytes; }
    byteSwap((char*) &bytes, sizeof(bytes));
    return bytes;
}

inline size_t netToHost(size_t bytes) {
	if (isBigEndian()) {
		return bytes;
	}
	byteSwap((char*)&bytes, sizeof(bytes));
	return bytes;
}