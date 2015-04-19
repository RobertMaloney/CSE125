#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\GameClient.h"
#include "graphics\GraphicsEngine.h"



using std::cout;


static void keyCallback(int key, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		GraphicsEngine::CloseGame();
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		GraphicsEngine::MoveUp();
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		GraphicsEngine::MoveLeft();
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		GraphicsEngine::MoveDown();
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		GraphicsEngine::MoveRight();
}

// used for debugging
void fillWithDebugPackets(deque<Packet> & packets) {
    Packet p;
    for (int i = 0; i < 26; ++i) {
        p.resize(0);
        int len = rand() % 100;
        for (int j = 0; j < len; ++j) {
            p.push_back('a' + i);
        }
        packets.push_back(p);
    }
}



int main(int argc, char* argv[]) {

	GraphicsEngine::Initialize();
	GraphicsEngine::SetKeyCallback(keyCallback);
    GameClient* client = new GameClient();
    client->Initialize();
    
    deque<Packet> packets;
    deque<Packet> updates;
    fillWithDebugPackets(packets);
	
	int i = 0;
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
        client->SendEvents(packets);
        client->ReceiveUpdates(updates);
        updates.clear();
	}
	
	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}