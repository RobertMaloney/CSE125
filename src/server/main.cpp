
#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "GameServer.h"

#include "utility\Event.h"

using std::cout;

int main(int argc, char* argv[]) {

    GameServer server;

    server.Initialize(1);
    server.Run();

    return 0;
}