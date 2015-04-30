#include "IdGenerator.h"

IdGenerator & IdGenerator::getInstance(){
	static IdGenerator idGen;
	return idGen;

}

void IdGenerator::update(ObjectId current){
	if (current > this->numOfObjects){
		this->numOfObjects = current;
	}
}

ObjectId IdGenerator::getNextId(){
	return this->numOfObjects + 1;
}

