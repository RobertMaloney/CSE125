#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <deque>
#include <functional>
#include <unordered_map>

#include "Packet.h"
#include "../utility/Event.h"

using std::deque;
using std::function;
using std::unordered_map;


typedef unsigned int ObjectId;
static ObjectId nextObjId;

typedef function <void(ObjectId, Event)> EventHandler;

class PacketHandler {

public:

    PacketHandler();
    ~PacketHandler();

    void dispatch(ObjectId clientId, deque<Packet> & received);

private:

    unordered_map<EventType, EventHandler> eventHandlers;

};


#endif