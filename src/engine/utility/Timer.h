#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <string>

class Timer {
private:
   clock_t startTime;
   int maxSeconds;

public:
   Timer() : Timer(60) {};
   Timer(int maxSec);

   void start();
   void reset();
   void init(int);
   void setStop(int seconds);
   bool atMax();
   double getTimeInSeconds();
   double getTimeInSecondsRemaining();
   std::string getTimeText();

   int getMin();
   int getSec();

   int getMinRemaining();
   int getSecRemaining();
};

#endif