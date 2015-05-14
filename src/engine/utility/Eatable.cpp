#include "Eatable.h"

Eatable::Eatable(int points)
{
   this->points = points;
}

int Eatable::getPoints()
{
   return this->points;
}

void Eatable::setPoints(int points)
{
   this->points = points;
}

