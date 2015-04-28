#include "InputHandler.h"


deque<Packet> InputHandler::input;

void InputHandler::handleKey(int key, int action, int mods)
{
	Packet p;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		GraphicsEngine::CloseGame();
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move forward" << endl;
		p.writeByte(EventType::MOVE_FORWARD);
		input.push_back(p);
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move left" << endl;
		p.writeByte(EventType::MOVE_LEFT);
		input.push_back(p);
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move backward" << endl;
		p.writeByte(EventType::MOVE_BACKWARD);
		input.push_back(p);;
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		cout << "client move right" << endl;
		p.writeByte(EventType::MOVE_RIGHT);
		input.push_back(p);
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
	else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::MoveUp();
	}
	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::MoveDown();
	}
	input.push_back(p);
}

