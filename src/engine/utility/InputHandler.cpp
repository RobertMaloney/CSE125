#include "InputHandler.h"

deque<Packet> InputHandler::input;

void InputHandler::handleKey(int key, int action, int mods)
{
	Packet p;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		GraphicsEngine::CloseGame();
	else if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			cout << "client move forward" << endl;
			p.writeByte(EventType::MOVE_FORWARD);
			input.push_back(p);
		}
		else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_FORWARD);
			input.push_back(p);
		}
	}
	else if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			cout << "client move left" << endl;
			p.writeByte(EventType::MOVE_LEFT);
			input.push_back(p);
		}
		else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_LEFT);
			input.push_back(p);
		}
	}
	else if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			cout << "client move backward" << endl;
			p.writeByte(EventType::MOVE_BACKWARD);
			input.push_back(p);
		}
		else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_BACKWARD);
			input.push_back(p);
		}
	}
	else if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			cout << "client move right" << endl;
			p.writeByte(EventType::MOVE_RIGHT);
			input.push_back(p);
		}
		else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_RIGHT);
			input.push_back(p);
		}
	}
	else if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::ScaleUp();
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		GraphicsEngine::ScaleDown();
	}


}
