
#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\GameServer.h"

using std::cout;

int main(int argc, char* argv[]) {

    GameServer server;

    server.Initialize();
    server.Run();


    return 0;
}