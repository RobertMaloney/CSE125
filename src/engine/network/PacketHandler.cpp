#include "PacketHandler.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
using std::function;
using glm::mat4;
using glm::vec3;
#include <iostream>

void forwardHandler(ObjectId id) {
    GameObject* player = ObjectDB::getInstance().get(id);

    if (player != nullptr) {
        player->location = glm::translate(player->location, vec3(0.f, -1.f, 0.f));
    }
}


void backwardHandler(ObjectId id) {
    GameObject* player = ObjectDB::getInstance().get(id);

    if (player != nullptr) {
        player->location = glm::translate(player->location, vec3(0.f, 1.f, 0.f));
    }
}


void leftHandler(ObjectId id) {
    GameObject* player = ObjectDB::getInstance().get(id);

    if (player != nullptr) {
        player->location = glm::translate(player->location, vec3(1.f, 0.f, 0.f));
    }
}


void rightHandler(ObjectId id) {
    GameObject* player = ObjectDB::getInstance().get(id);

    if (player != nullptr) {
        player->location = glm::translate(player->location, vec3(-1.f, 0.f, 0.f));
    }
}


PacketHandler::PacketHandler(){
	eventHandlers[EventType::MOVE_FORWARD] = EventHandler(forwardHandler);
	eventHandlers[EventType::MOVE_BACKWARD] = EventHandler(backwardHandler);
	eventHandlers[EventType::MOVE_LEFT] = EventHandler(leftHandler);
	eventHandlers[EventType::MOVE_RIGHT] = EventHandler(rightHandler);
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


