#include "PacketHandler.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
using std::function;
using glm::mat4;
using glm::vec3;
#include <iostream>


void forwardHandler(ObjectId id) {
  //  //Player* player = dynamic_cast<Player*>(ObjectDB::getInstance().get(id));/////??????????????
  //  GameObject* player = ObjectDB::getInstance().get(id);

  //  if (player != nullptr) {
  //      //player->location = glm::translate(player->location, vec3(0.f, -1.f, 0.f));
		//std::cout << "Player " << id << " moving forward\n";
		//float dir = player->getLoc().w;
		//player->getLoc().b += glm::cos(glm::radians(dir));
		//player->getLoc().g += glm::sin(glm::radians(dir));

  //  }

	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, true);
}

void stopForwardHandler(ObjectId id) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, false);
}

void backwardHandler(ObjectId id) {
 //   GameObject* player = ObjectDB::getInstance().get(id);

 //   if (player != nullptr) {
 //       //player->location = glm::translate(player->location, vec3(0.f, 1.f, 0.f));
	//	std::cout << "Player " << id << " moving back\n";
	//	float dir = player->getLoc().w;
	//	player->getLoc().b -= glm::cos(glm::radians(dir));
	//	player->getLoc().g -= glm::sin(glm::radians(dir));
	//}

	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, true);
}

void stopBackwardHandler(ObjectId id) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, false);
}

void leftHandler(ObjectId id) {
  //  GameObject* player = ObjectDB::getInstance().get(id);

  //  if (player != nullptr) {
  //      //player->location = glm::translate(player->location, vec3(1.f, 0.f, 0.f));
		//std::cout << "Player " << id << " moving left\n";
		//player->getLoc().a += 1.f;
  //  }

	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, true);
}

void stopLeftHandler(ObjectId id) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, false);
}

void rightHandler(ObjectId id) {
  //  GameObject* player = ObjectDB::getInstance().get(id);

  //  if (player != nullptr) {
  //      //player->location = glm::translate(player->location, vec3(-1.f, 0.f, 0.f));
		//std::cout << "Player " << id << " moving right\n";
		//player->getLoc().a -= 1.f;
  //  }

	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, true);
}

void stopRightHandler(ObjectId id) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, false);
}

PacketHandler::PacketHandler(){
	eventHandlers[EventType::MOVE_FORWARD] = EventHandler(forwardHandler);
	eventHandlers[EventType::MOVE_BACKWARD] = EventHandler(backwardHandler);
	eventHandlers[EventType::MOVE_LEFT] = EventHandler(leftHandler);
	eventHandlers[EventType::MOVE_RIGHT] = EventHandler(rightHandler);

	eventHandlers[EventType::STOP_FORWARD] = EventHandler(stopForwardHandler);
	eventHandlers[EventType::STOP_BACKWARD] = EventHandler(stopBackwardHandler);
	eventHandlers[EventType::STOP_LEFT] = EventHandler(stopLeftHandler);
	eventHandlers[EventType::STOP_RIGHT] = EventHandler(stopRightHandler);
}


PacketHandler::~PacketHandler() {

}


void PacketHandler::dispatch(ObjectId clientId, deque<Packet> & received) {
	for (auto it = received.begin(); it != received.end(); ++it) {
		if (it->size() <= 0) {
			continue;
		}

		EventType eventType = static_cast<EventType>(it->at(0));

		if (eventHandlers.find(eventType) != eventHandlers.end()) {
            eventHandlers[eventType](clientId);
        }
	}
}




