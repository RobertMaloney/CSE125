#include "PacketHandler.h"



void forwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, true);
	player->setParticle(true);
}


void stopForwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::UP, false);
	player->setParticle(false);
}


void backwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, true);
	player->setParticle(true);
}


void stopBackwardHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::DOWN, false);
	player->setParticle(false);
    }


void leftHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, true);
	player->setParticle(true);
}


void stopLeftHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::LEFT, false);
	player->setParticle(false);
    }


void rightHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, true);
	player->setParticle(true);
}


void stopRightHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	player->setMoving(Player::RIGHT, false);
	player->setParticle(false);
}

void lookHandler(ObjectId id, Packet &p) {
	Player* player = static_cast<Player*> (ObjectDB::getInstance().get(id));
	p.readByte(); // throwaway event
	player->moveAngle(p.readFloat()); // x
	p.readFloat(); // y
}

void jumpHandler(ObjectId id, Packet & p) {
	Player* player = dynamic_cast<Player*>(ObjectDB::getInstance().get(id));
    player->setMoving(Player::JUMP, true);
	player->setParticle(true);
}


void stopJumpHandler(ObjectId id, Packet & p) {
	Player* player = dynamic_cast<Player*>(ObjectDB::getInstance().get(id));
	player->setMoving(Player::JUMP, false);
	player->setParticle(false);
}


void replayHandler(ObjectId id, Packet & p) {
	GameState::getInstance().reset(id);
}

void loadHandler(ObjectId id, Packet & p) {
	GameServer::loadDone = true;
}

void pauseHandler(ObjectId id, Packet & p) {
	if (!GameServer::pause){
		GameServer::pause = true;
		GameServer::pauseTrue = true;
	}
}

void continueHandler(ObjectId id, Packet & p) {
	if (GameServer::pause){
		GameServer::pause = false;
		GameServer::continueTrue = true;
	}
}


PacketHandler::PacketHandler(){
	eventHandlers[EventType::MOVE_FORWARD] = EventHandler(forwardHandler);
	eventHandlers[EventType::MOVE_BACKWARD] = EventHandler(backwardHandler);
	eventHandlers[EventType::MOVE_LEFT] = EventHandler(leftHandler);
	eventHandlers[EventType::MOVE_RIGHT] = EventHandler(rightHandler);
	eventHandlers[EventType::JUMP] = EventHandler(jumpHandler);

	eventHandlers[EventType::STOP_FORWARD] = EventHandler(stopForwardHandler);
	eventHandlers[EventType::STOP_BACKWARD] = EventHandler(stopBackwardHandler);
	eventHandlers[EventType::STOP_LEFT] = EventHandler(stopLeftHandler);
	eventHandlers[EventType::STOP_RIGHT] = EventHandler(stopRightHandler);
	eventHandlers[EventType::STOP_JUMP] = EventHandler(stopJumpHandler);

	eventHandlers[EventType::LOOK] = EventHandler(lookHandler);
	eventHandlers[EventType::REPLAY] = EventHandler(replayHandler);
	eventHandlers[EventType::LOAD_END] = EventHandler(loadHandler);
	eventHandlers[EventType::PPAUSE] = EventHandler(pauseHandler);
	eventHandlers[EventType::CONTINUE] = EventHandler(continueHandler);
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




