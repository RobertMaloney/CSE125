#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\GameClient.h"
#include "graphics\GraphicsEngine.h"


using std::cout;

bool DEBUG = true;
GameClient* client;

static void keyCallback(int key, int action, int mods) {
	deque<Packet> packets;
	Packet p;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		GraphicsEngine::CloseGame();
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move forward" << endl;
		//GraphicsEngine::MoveUp();
		p.WriteByte((byte)0);
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move left" << endl;
		//GraphicsEngine::MoveLeft();
		p.WriteByte((byte)1);
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move backward" << endl;
		//GraphicsEngine::MoveDown();
		p.WriteByte((byte)2);
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move right" << endl;
		//GraphicsEngine::MoveRight();
		p.WriteByte((byte)3);
	}
	packets.push_back(p);
	client->SendEvents(packets);
}

// used for debugging
void fillWithDebugPackets(deque<Packet> & packets) {
    Packet p;
    for (int i = 0; i < 26; ++i) {
        p.Resize(0);
        int len = rand() % 1000;
        for (int j = 0; j < len; ++j) {
            p.WriteChar('a' + i);
        }
        packets.push_back(p);
    }
}


int main(int argc, char* argv[]) {

	GraphicsEngine::Initialize();
	GraphicsEngine::SetKeyCallback(keyCallback);
    client = new GameClient();
    client->Initialize();
    
    deque<Packet> packets;
    deque<Packet> updates;
    fillWithDebugPackets(packets);
	
	int i = 0;
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		if (DEBUG) {
			//client->SendEvents(packets);
			client->ReceiveUpdates(updates);
			//client->PrintUpdates(updates);
			GraphicsEngine::UpdatePlayer(updates);
			updates.clear();
		}
	}
	
	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}