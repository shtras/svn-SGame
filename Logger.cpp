#include "StdAfx.h"
#include "Logger.h"
#include <time.h>

Logger& Logger::getInstance()
{
  static Logger instance;
  return instance;
}

Logger::Logger()
{

}

Logger::~Logger()
{
  map<CString, ofstream*>::iterator itr = logs_.begin();
  for (; itr != logs_.end(); ++itr) {
    ofstream* strm = itr->second;
    strm->close();
    delete strm;
  }
}

void Logger::log(CString logName, CString message)
{
  time_t seconds = time(NULL);
  char* timeStr = ctime(&seconds);
  timeStr[strlen(timeStr)-1] = 0;
  ofstream* myStream = NULL;
  if (logs_.count(logName) == 0) {
    myStream = new ofstream();
    myStream->open(logName);
    logs_[logName] = myStream;
  } else {
    myStream = logs_[logName];
  }
  *myStream << timeStr << " " << message << endl;
}
