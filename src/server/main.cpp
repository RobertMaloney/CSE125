#include "GameServer.h"
#include "utility\Event.h"

using std::cout;


int main(int argc, char* argv[]) {
	GameServer server;
	server.initialize(4);
    server.run();

    return 0;
}