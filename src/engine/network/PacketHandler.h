#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <deque>
#include <glm.hpp>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <gtc\matrix_transform.hpp>

#include "Packet.h"
#include "../utility/Event.h"
#include "../utility/ObjectDB.h"
#include "../utility/Player.h"
#include "../utility/GameEngine.h"


using glm::mat4;
using glm::vec3;
using std::deque;
using std::function;
using std::unordered_map;

typedef function <void(ObjectId)> EventHandler;

class PacketHandler {

public:

    PacketHandler();
    ~PacketHandler();

    void dispatch(ObjectId clientId, vector<Packet> & received);

private:

    unordered_map<EventType, EventHandler> eventHandlers;

};


#endif