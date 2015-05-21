#include "Resource.h"

Resource::Resource(Model rm, float radius, float theta, float azimuth, float direction) 
: GameObject(radius, theta, azimuth, direction) {
   this->rm = rm;
   this->modelRadius = 5.f;
}

