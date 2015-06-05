#include "Resource.h"
// These dimensions are whole length not radii. they are approximate measured in blender.
// tree      xy 6.f z 16.f
// trunk     xyz      4.f
// rock      xy 8.f z 4.f
// mushroom  xy 2.f z 4.f
// float     xy 2.f z 1.5f
Resource::Resource(Model rm, float radius, float theta, float azimuth, float direction) 
: GameObject(radius, theta, azimuth, direction) {
   this->rm = rm;
}


void Resource::loadConfiguration(Json::Value config) {
	this->visible = true;
}
