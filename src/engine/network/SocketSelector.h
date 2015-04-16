#ifndef SOCKET_SELECTOR_H
#define SOCKET_SELECTOR_H

#include <vector>
#include <unordered_map>

#include "Socket.h"

using std::vector;
using std::unordered_map;

class SocketSelector {

public:

    SocketSelector();
    SocketSelector(int secs, int usecs);
    ~SocketSelector();

    void Add(Socket* socket);
    void Remove(Socket* socket);
    void GetReadable(vector<Socket*> & sockets);
    void GetWriteable(vector<Socket*> & sockets);
    void GetExceptional(vector<Socket*> & sockets);

private:

    int maxFd;
    fd_set mReadSet;
    fd_set mWriteSet;
    fd_set mExceptSet;
    timeval masterTimeout;
    unordered_map<int, Socket*> socketSet;

};


#endif