#include "StdAfx.h"
#include "Time.h"


Time& Time::getTime()
{
  static Time instance;
  return instance;
}

Time::Time():year_(2317),month_(7),day_(12),hour_(5),minute_(30),second_(0),fracture_(0),control_(NULL)
{

}

Time::~Time()
{

}

void Time::progress(int value)
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

int Time::getYear()
{
  return year_;
}

int Time::getMonth()
{
  return month_;
}

int Time::getDay()
{
  return day_;
}

int Time::getHour()
{
  return hour_;
}

int Time::getMinute()
{
  return minute_;
}

int Time::getSecond()
{
  return second_;
}

CString Time::getTimeStr()
{
  return CString(day_, 2) + "/" + CString(month_,2) + "/" + CString(year_) + " " + CString(hour_,2) + ":" + CString(minute_,2) + ":" + CString(second_,2);
}
