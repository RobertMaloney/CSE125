#include "Resource.h"

Resource::Resource(ResourceModel rm, int points, float radius, float theta, float azimuth, float direction) 
   : GameObject(radius, theta, azimuth, direction)
{
   this->rm = rm;
   this->points = points;
}

int Resource::getPoints()
{
   return this->points;
}

void Resource::setPoints(int points)
{
   this->points = points;
}

