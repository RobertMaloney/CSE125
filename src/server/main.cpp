
#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\TCPListener.h"
#include "network\TCPConnection.h"

using std::cout;

int main(int argc, char* argv[]) {

    TCPListener listener;
    TCPConnection* nclient = nullptr;

	Socket::Initialize();
    listener.Bind(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    listener.Listen(5);

	Packet p;
    char buffer[1024];
    int numRecvd = 0;

    while (true) {
        memset((void*) &p, 0, p.size());
        if (!nclient) {
            nclient = listener.Accept();
			std::cout << "accepted new client" << std::endl;
        }
		nclient->Receive(p);
        
        std::cout << "Packet : ";
		for (auto it = p.begin(); it != p.end(); ++it){
			std::cout << std::to_string(*it);
		}
        p.resize(0);
		std::cout << "" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}