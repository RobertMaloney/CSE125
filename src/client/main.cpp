#include <thread>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "network\TCPListener.h"
#include "network\TCPConnection.h"

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

int main(int argc, char* argv[]) {

	GraphicsEngine::Initialize();
	GraphicsEngine::SetKeyCallback(keyCallback);

	Socket::Initialize();
	TCPConnection client;
	char buffer[1024];

    srand(time(NULL));
	memset((void*)&buffer, 0, 1024);
	client.Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
	const char * echo = "echo..";
	Packet p;
	vector<Packet> packets;
	for (int i = 0; i < 26; ++i){
		p.resize(0);
        int len = rand() % 100;
        for (int j = 0; j < len; ++j) {
			p.push_back('a' + i);
		}
		packets.push_back(p);
	}
	
	
	int i = 0;
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		Packet p = packets[i % 26];
		client.Send(p);
		++i;
        std::cout << i << " size : " << p.size() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}