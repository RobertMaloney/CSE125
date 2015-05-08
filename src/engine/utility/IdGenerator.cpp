#include "IdGenerator.h"

IdGenerator & IdGenerator::getInstance(){
	static IdGenerator idGen;
	return idGen;

}

ObjectId IdGenerator::createId() {
	int id = numOfObjects++;
	return id;
}