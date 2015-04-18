#include "Location.h"


Location::Location(int nx, int ny, int nz){
	this->x = nx;
	this->y = ny;
	this->z = nz;
}

int Location::getX() {
	return x;
}

int Location::getY() {
	return y;
}

int Location::getZ() {
	return z;
}

void Location::setX(int newX) {
	x = newX;
}

void Location::setY(int newY) {
	y = newY;
}

void Location::setZ(int newZ) {
	z = newZ;
}