#include "timer.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

static inline
double seconds(const struct timeval &time) {
  return ((double)time.tv_sec) + (((double)time.tv_usec)/1000000.0);
}

Timer::Timer() 
  : userTime(0.0)
  , systemTime(0.0)
  , wallTime(0.0)
{}

void Timer::start() 
{
  struct rusage usage;
  struct timeval todTime;
  if (0 != getrusage(RUSAGE_SELF, &usage)) {
    perror("getrusage failed");
    exit(1);
  }
  if (0 != gettimeofday(&todTime, 0)) {
    perror("gettimeofday failed");
    exit(1);
  }

  userTime = seconds(usage.ru_utime);
  systemTime = seconds(usage.ru_stime);
  wallTime = seconds(todTime);
}

void Timer::stop() 
{
  struct rusage usage;
  struct timeval todTime;
  if (0 != getrusage(RUSAGE_SELF, &usage)) {
    perror("getrusage failed");
    exit(1);
  }
  if (0 != gettimeofday(&todTime, 0)) {
    perror("gettimeofday failed");
    exit(1);
  }
  userTime = seconds(usage.ru_utime)-userTime;
  systemTime = seconds(usage.ru_stime)-systemTime;
  wallTime = seconds(todTime)-wallTime;
}

double Timer::user() {
  return userTime;
}
double Timer::system() {
  return systemTime;
}

double Timer::total() {
  return userTime+systemTime;
}

double Timer::wall() {
  return wallTime;
}
