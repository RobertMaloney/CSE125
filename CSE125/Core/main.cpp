
// STL
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "..\Graphics\GraphicsEngine.h"

using namespace std;

static void keyCallback(int key, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		GraphicsEngine::CloseGame();
}

int
main(int argc, char** argv) {
	GraphicsEngine::Initialize();
	GraphicsEngine::SetKeyCallback(keyCallback);
	
	while (!GraphicsEngine::Closing()) {

		GraphicsEngine::DrawAndPoll();
	}

	GraphicsEngine::Destroy();
	system("pause");
	return 0;
}

