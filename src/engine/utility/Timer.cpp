#include "Timer.h"

Timer::Timer(int maxSec)
{
	init(maxSec);
}

void Timer::init(int maxSec){
	//this->waitSeconds = 0;
	this->startTime = clock();
	this->maxSeconds = maxSec;
}

void Timer::start() {
   this->startTime = clock();
}

void Timer::reset() {
	this->init(this->maxSeconds);
}

double Timer::getTimeInSeconds() {
   clock_t endTime = clock();
   clock_t clockTicksTaken = endTime - startTime;
   double timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;

   return timeInSeconds >= maxSeconds ? maxSeconds : timeInSeconds;
}

double Timer::getTimeInSecondsRemaining() {
   clock_t endTime = clock();
   clock_t clockTicksTaken = endTime - startTime;
   double timeInSeconds = maxSeconds - (clockTicksTaken / (double)CLOCKS_PER_SEC);
   
   return timeInSeconds <= 0 ? 0 : timeInSeconds;
}

std::string Timer::getTimeText() {
   double timeInSeconds = getTimeInSeconds();
   int minutes = (int)timeInSeconds / 60;
   int seconds = (int)timeInSeconds % 60;

   char buffer[50];
   sprintf(buffer, "%01d:%02d", minutes, seconds);
   return buffer;
}

void Timer::pause(){
	this->pauseStart = clock();
}
void Timer::restart(){
	clock_t endTime = clock();
	clock_t clockTicksTaken = endTime - pauseStart;
	this->startTime = this->startTime + clockTicksTaken;
	//this->waitSeconds = 0;
}
int Timer::getMin() {
	double timeInSeconds = getTimeInSeconds();
	return (int)timeInSeconds / 60;
}

int Timer::getSec() {
	double timeInSeconds = getTimeInSeconds();
	return (int)timeInSeconds % 60;
}

int Timer::getMinRemaining() {
   double timeInSeconds = getTimeInSecondsRemaining();
   return (int)timeInSeconds / 60;
}

int Timer::getSecRemaining() {
   double timeInSeconds = getTimeInSecondsRemaining();
   return (int)timeInSeconds % 60;
}

bool Timer::atMax()
{
   return getTimeInSeconds() == maxSeconds;
}