#include "StdAfx.h"
#include "auxFuncs.h"

double RadToDeg(double rad)
{
  return rad * 180.0 / PI;
}

double DegToRad(double deg)
{
  return deg * PI / 180.0;
}

bool isNumberChar(int key)
{
  if (key >= 'a' && key <= 'z') {
    return true;
  }
  if (key >= 'A' && key <= 'Z') {
    return true;
  }
  return false;
}

bool isLetterChar(int key)
{
  if (key >= 0x30 && key <= 0x39) {
    return true;
  }
  return false;
}

void checkErrorDebug(CString errorMsg/* = ""*/)
{
  GLenum err = glGetError();
  if (err != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, errorMsg + CString(" OpenGL error: ") + CString((int)err));
  }
  assert(err == 0);
}

void checkReleaseError(CString errorMsg)
{
  checkErrorDebug(errorMsg);
}

int parseInt(CString str)
{
  if (str.getSize() == 0) {
    return 0;
  }
  int res = 0;
  bool neg = false;
  int start = 0;
  if (str[0] == '-') {
    neg = true;
    ++start;
  }
  for (int i=start; i<str.getSize(); ++i) {
    char c = str[i];
    if (c < '0' || c > '9') {
      Logger::getInstance().log(ERROR_LOG_NAME, "Error parsing integer: " + str);
      return 0;
    }
    int digit = c - '0';
    res += digit;
    res *= 10;
  }
  res /= 10;
  if (neg) {
    res = -res;
  }
  return res;
}
