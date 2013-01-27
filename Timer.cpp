#include "StdAfx.h"
#include "Timer.h"


Timer& Timer::getTime()
{
  static Timer instance;
  return instance;
}

Timer::Timer():year_(2317),month_(7),day_(12),hour_(5),minute_(30),second_(0),fracture_(0),control_(NULL)
{

}

Timer::~Timer()
{

}

void Timer::progress(int value)
{
  fracture_ += value;
  while (fracture_ > 100) {
    fracture_ -= 100;
    ++second_;
  }
  while (second_ > 60) {
    second_ -= 60;
    ++minute_;
  }
  while (minute_ > 60) {
    minute_ -= 60;
    ++hour_;
  }
  while (hour_ > 24) {
    hour_ -= 24;
    ++day_;
  }
  while (day_ > 30) {
    day_ -= 30;
    ++month_;
  }
  while (month_ > 12) {
    month_ -= 12;
    ++year_;
  }
  if (control_) {
    control_->progressTime();
  }
}

int Timer::getYear()
{
  return year_;
}

int Timer::getMonth()
{
  return month_;
}

int Timer::getDay()
{
  return day_;
}

int Timer::getHour()
{
  return hour_;
}

int Timer::getMinute()
{
  return minute_;
}

int Timer::getSecond()
{
  return second_;
}

CString Timer::getTimeStr()
{
  return CString(day_, 2) + "/" + CString(month_,2) + "/" + CString(year_) + " " + CString(hour_,2) + ":" + CString(minute_,2) + ":" + CString(second_,2);
}

int Timer::getWatch()
{
  if (hour_ < 8) {
    return 0;
  }
  if (hour_ < 16) {
    return 1;
  }
  return 2;
}
