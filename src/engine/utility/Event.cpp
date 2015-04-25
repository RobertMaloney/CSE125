#include "Event.h"

Event::Event() {

}


Event::Event(EventType t) {
	this->type = t;
}


Event::~Event() {

}


void Event::setType(EventType t) {
	this->type = t;
}


EventType Event::getType() {
	return type;
}


void Event::serialize(Packet & p) {
	p.writeByte(static_cast<byte>(type));
}


void Event::deserialize(Packet & p) {
	this->type = static_cast<EventType>(p.readByte());
}