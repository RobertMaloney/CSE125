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

	TCPConnection client;
	char buffer[1024];

	memset((void*)&buffer, 0, 1024);
	client.Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
	const char * echo = "echo..";

	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		client.Send(echo, 7);
		client.Receive(static_cast<void*>(&buffer), 1024);

	}

	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}