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

	memset((void*)&buffer, 0, 1024);
	//client.Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
	const char * echo = "echo..";
	
	vector<Packet> packets(26);
	for (int i = 0; i < 26; ++i){
		Packet p;
		p.push_back('a' + i);
		packets[i] = p;

	}
	

	for (unsigned int i = 0; i < packets.size(); ++i){
		for (auto it = packets[i].begin(); it != packets[i].end(); ++it){
			std::cout << *it << " ";
			std::cout << packets[i].size() << std::endl;
			std::cout << packets.size() << std::endl;
		}
	}

	std::cout << " ad " << packets.size() << std::endl;
	
	int i = 0;
	while (!GraphicsEngine::Closing()) {
		//GraphicsEngine::DrawAndPoll();
		std::cout << packets.size() << std::endl;
		Packet p = packets[i % 26];
		
		std::cout << p.size() << std::endl;
		std::cout << p.at(0) << std::endl;
		client.Send(packets[i % 26]);
		++i;
		std::cout << i << std::endl;
	}
	
	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}