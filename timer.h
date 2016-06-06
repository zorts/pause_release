// -*- C++ -*-
#ifndef TIMER_H_
#define TIMER_H_

class Timer {
public:
  Timer();
  // default dtor OK

  void start();
  void stop();

  double user();
  double system();
  double total();
  double wall();

private:
  // prohibit copy/assign; do not implement
  Timer(const Timer& rhs);
  Timer& operator=(const Timer& rhs);

  double userTime, systemTime, wallTime;
};

#endif //  TIMER_H_
