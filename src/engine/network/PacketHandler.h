#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <deque>
#include <functional>
#include <unordered_map>

#include "Packet.h"
#include "../utility/Event.h"
#include "../utility/ObjectDB.h"
#include "../utility/Player.h"

using std::deque;
using std::function;
using std::unordered_map;

typedef function <void(ObjectId)> EventHandler;

class PacketHandler {

public:

    PacketHandler();
    ~PacketHandler();

    void dispatch(ObjectId clientId, deque<Packet> & received);

private:

    unordered_map<EventType, EventHandler> eventHandlers;

};


#endif