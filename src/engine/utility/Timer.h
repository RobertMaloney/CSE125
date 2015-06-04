#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <string>

class Timer {
private:
   clock_t startTime;
   int maxSeconds;

public:
   Timer();

   void start();
   void reset();
   void setStop(int seconds);
   bool atMax();
   double getTimeInSeconds();
   std::string getTimeText();
   static Timer & getInstance();
};

#endif