#include "Resource.h"

Resource::Resource(ResourceModel rm, int points)
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

