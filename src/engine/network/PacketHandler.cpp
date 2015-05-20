#include "PacketHandler.h"



void forwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, true);
}


void stopForwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, false);
}


void backwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, true);
}


void stopBackwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, false);
    }


void leftHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, true);
}


void stopLeftHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, false);
    }


void rightHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, true);
}


void stopRightHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, false);
}

void lookHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	p.readByte(); // throwaway event
	player->moveAngle(p.readFloat()); // x
	p.readFloat(); // y
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

	eventHandlers[EventType::LOOK] = EventHandler(lookHandler);
}


PacketHandler::~PacketHandler() {

}


void PacketHandler::dispatch(ObjectId clientId, vector<Packet> & received) {
	for (auto it = received.begin(); it != received.end(); ++it) {
		if (it->size() <= 0) {
			continue;
		}

		EventType eventType = static_cast<EventType>(it->at(0));

		if (eventHandlers.find(eventType) != eventHandlers.end()) {
            eventHandlers[eventType](clientId, *it);
        }
	}
}




