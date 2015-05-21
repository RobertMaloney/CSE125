#include "Timer.h"

clock_t Timer::startTime;

void Timer::start() {
   startTime = clock();
}

void Timer::reset() {
   startTime = clock();
}

double Timer::getTimeInSeconds() {
   clock_t endTime = clock();
   clock_t clockTicksTaken = endTime - startTime;
   double timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;

   return timeInSeconds;
}

std::string Timer::getTimeText() {
   double timeInSeconds = getTimeInSeconds();
   int minutes = (int) timeInSeconds / 60;
   int seconds = (int) timeInSeconds % 60;
   int fraction = (int)(timeInSeconds * 100) % 100;

   char buffer[50];
   sprintf(buffer, "%01.0f:%02.0f:%02.2f", minutes, seconds, fraction);
   return buffer;
}