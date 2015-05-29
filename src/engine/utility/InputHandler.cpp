#include "InputHandler.h"
#include "..\graphics\GraphicsEngine.h"
#include "GameSound.h"

vector<Packet> InputHandler::input;
vector<Packet> InputHandler::clientInput;

void InputHandler::handleKey(int key, int action, int mods) {
	Packet p;

	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		//GraphicsEngine::CloseGame();
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			cout << "client move forward" << endl;
			p.writeByte(EventType::MOVE_FORWARD);
			input.push_back(p);
			clientInput.push_back(p);
		} else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_FORWARD);
			input.push_back(p);
			clientInput.push_back(p);
		}
	} else if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			cout << "client move left" << endl;
			p.writeByte(EventType::MOVE_LEFT);
			input.push_back(p);
			clientInput.push_back(p);
		} else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_LEFT);
			input.push_back(p);
			clientInput.push_back(p);
		}
	} else if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			cout << "client move backward" << endl;
			p.writeByte(EventType::MOVE_BACKWARD);
			input.push_back(p);
			clientInput.push_back(p);
		} else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_BACKWARD);
			input.push_back(p);
			clientInput.push_back(p);
		}
	} else if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			cout << "client move right" << endl;
			p.writeByte(EventType::MOVE_RIGHT);
			input.push_back(p);
			clientInput.push_back(p);
		} else if (action == GLFW_RELEASE) {
			p.writeByte(EventType::STOP_RIGHT);
			input.push_back(p);
			clientInput.push_back(p);
		}
	} else if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			cout << "jumping" << endl;
			p.writeByte(EventType::JUMP);
			input.push_back(p);
			clientInput.push_back(p);
		} else if (action == GLFW_RELEASE) {
			cout << "stop jumping" << endl;
			p.writeByte(EventType::STOP_JUMP);
			input.push_back(p);
			clientInput.push_back(p);
		}
	}
	else if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		GraphicsEngine::ScaleUp();
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		GraphicsEngine::ScaleDown();
	}
	//ENTER
	else if (key == GLFW_KEY_ENTER) {
		if (action == GLFW_PRESS) {
			cout << "client confirm" << endl;
			p.writeByte(EventType::CONFIRM);
			input.push_back(p);
		}
	}
	// Plus Zoom in
	else if (key == GLFW_KEY_EQUAL) { //+
		if (action == GLFW_PRESS) {
			cout << "zoom in" << endl;
			GraphicsEngine::ZoomIn();
		}
	}
	// Minus zoom out
	else if (key == GLFW_KEY_MINUS) {//=
		if (action == GLFW_PRESS) {
			cout << "zoom out" << endl;
			GraphicsEngine::ZoomOut();
		}
	}
	// Escape pause
	else if (key == GLFW_KEY_ESCAPE) {//Esc
		if (action == GLFW_PRESS) {
			cout << "Game Pause" << endl;
			//GraphicsEngine::ZoomOut();
		    GameClient::inMenu = true;
			GraphicsEngine::setCursor(GLFW_CURSOR_NORMAL);
			MenuState::pause_flag = true;
			MenuState::submit = false;
			GraphicsEngine::setMenuStatus(MenuStatus::MCONTINUE);
		}
	}
}

void InputHandler::handleMouse(float x, float y)
{
	Packet p;
	p.writeByte(EventType::LOOK);
	p.writeFloat(x);
	p.writeFloat(y);
	input.push_back(p);
}
