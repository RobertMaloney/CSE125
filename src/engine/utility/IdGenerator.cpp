#include "IdGenerator.h"

IdGenerator & IdGenerator::getInstance(){
	static IdGenerator idGen;
	return idGen;

}

void IdGenerator::update(ObjectId current){
	//if (current > this->numOfObjects){
		//this->numOfObjects = current;
	//}
}

ObjectId IdGenerator::getNextId(){
	return this->numOfObjects = this->numOfObjects + 1;
}

ObjectId IdGenerator::createId() {
	int id = numOfObjects++;
	return id;
}
