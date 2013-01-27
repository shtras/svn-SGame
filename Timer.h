#pragma once
#include "TimeControl.h"

class Timer
{
public:
  static Timer& getTime();
  void progress(int value);
  int getYear();
  int getMonth();
  int getDay();
  int getHour();
  int getMinute();
  int getSecond();
  CString getTimeStr();
  void setTimeControl(TimeControl* control) {control_ = control;}
  int getWatch();
private:
  Timer();
  ~Timer();
  int year_;
  int month_;
  int day_;
  int hour_;
  int minute_;
  int second_;
  int fracture_;
  TimeControl* control_;
};