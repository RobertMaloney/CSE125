#include "PacketHandler.h"


PacketHandler::PacketHandler(){

}


PacketHandler::~PacketHandler() {}


void PacketHandler::dispatch(ObjectId clientId, deque<Packet> & received) {
    for (auto it = received.begin(); it != received.end(); ++it) {
        if (it->size() <= 0) {
            continue; 
        }

        EventType eventType = static_cast<EventType>(it->at(0));
        auto handler = eventHandlers.find(eventType);

        if (handler == eventHandlers.end()) {
            continue;
        }
        eventHandlers[eventType](clientId, Event());
    }
}