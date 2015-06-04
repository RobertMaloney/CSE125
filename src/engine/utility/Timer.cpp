#include "Timer.h"

Timer::Timer()
{
   this->startTime = clock();
   this->maxSeconds = 180;
}

void Timer::start() {
   this->startTime = clock();
}

void Timer::reset() {
   this->startTime = clock();
}

double Timer::getTimeInSeconds() {
   clock_t endTime = clock();
   clock_t clockTicksTaken = endTime - startTime;
   double timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;

   return timeInSeconds >= maxSeconds ? maxSeconds : timeInSeconds;
}

std::string Timer::getTimeText() {
   double timeInSeconds = getTimeInSeconds();
   int minutes = (int)timeInSeconds / 60;
   int seconds = (int)timeInSeconds % 60;

   char buffer[50];
   sprintf(buffer, "%01d:%02d", minutes, seconds);
   return buffer;
}

Timer & Timer::getInstance(){
   static Timer timer;
   return timer;
}

bool Timer::atMax()
{
   return getTimeInSeconds() == maxSeconds;
}