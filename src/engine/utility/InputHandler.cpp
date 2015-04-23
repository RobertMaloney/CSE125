#include "InputHandler.h"


void InputHandler::handleKey(int key, int action, int mods)
{
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
	else if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::ScaleUp();
	}
	else if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::ScaleDown();
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::RotateLeft();
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::RotateRight();
	}
	GameClient::input.push_back(p);
}
