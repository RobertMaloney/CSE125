#include "GameServer.h"
#include "utility\Event.h"

using std::cout;


int main(int argc, char* argv[]) {
	GameServer server;
	server.initialize();
    server.run();

    return 0;
}