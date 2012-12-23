#pragma once
#include <fstream>

class Logger
{
public:
  static Logger& getInstance();

  void log(CString logName, CString message);
private:
  Logger();
  ~Logger();

  map<CString, ofstream*> logs_;
};
