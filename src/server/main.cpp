
#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\TCPListener.h"
#include "network\TCPConnection.h"

#include "utility\Event.h"

using std::cout;

int main(int argc, char* argv[]) {

    TCPListener listener;
    TCPConnection* nclient = nullptr;

    listener.Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    listener.Listen(5);
    char buffer[1024];
    int numRecvd = 0;
    Event e;

    while (true) {
        if (!nclient) {
            nclient = listener.Accept();
        }
       numRecvd = nclient->Receive(static_cast<void*>(&buffer), 1024);
		if (numRecvd == 0){
			nclient->Close();
		}
		if (numRecvd > 0){
         //cout << buffer;
			cout << std::string(buffer) << std::endl;
         e.deserialize(buffer);
         if (e.theevent == EventType::MOVE_DOWN)
            cout << "down";
		}
       nclient->Send(static_cast<void*>(&buffer), numRecvd);
    }
    return 0;
}