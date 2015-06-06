
#include "GameServer.h"
#include "utility\Event.h"
#include <iomanip>

using std::cout;
using std::thread;

void stringToLowerCase(string & string) {
	transform(string.begin(), string.end(), string.begin(), ::tolower);
	

}

void trimNewLine(string & string) {
	for (int i = string.size() - 1; i >= 0; --i){
		if ((string[i] >= 'a' && string[i] <= 'z') ||
		    (string[i] >= 'A' && string[i] <= 'Z')) {
			string.resize(i + 1);
			break;
		}
	}
}

void execute(string & command, GameServer* server) {
	if (command == "reset") {
		server->reset();
	}
}


int main(int argc, char* argv[]) {
	string command;
	GameServer server;
	server.initialize();
	
	thread gameThread(&GameServer::run, &server);
	
	while (true) { 
		cout << "dustyplanet:-$ ";
		cin >> command;
		stringToLowerCase(command);
		trimNewLine(command);
		if (command == "exit") {
			server.stop();
			break;
		}

		execute(command, &server);
	}

	gameThread.join();
    
	return 0;
}
