#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <string>

class Timer {
private:
   static clock_t startTime;

public:
   static void start();
   static void reset();
   static double getTimeInSeconds();
   static std::string getTimeText();
};

#endif