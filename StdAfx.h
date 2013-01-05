#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <assert.h>
using namespace std;

#ifdef DEBUG
#define checkError(n) checkErrorDebug(n)
#else
#define checkError(n)
#endif
#define INFO_LOG_NAME "info.log"
#define ERROR_LOG_NAME "error.log"
#define PI (3.1415926536)

#ifdef DEBUG
#define BUILD_STR "Debug"
#else
#define BUILD_STR "Release"
#endif

#include "CString.h"
#include "Logger.h"
#include "auxFuncs.h"
#include "Vector.h"
#include "Matrix.h"
#include "SDL.h"
#include "SDL_image.h"
#include <GL/gl.h>

#include "sigslot.h"
using namespace sigslot;

struct Rect
{
  Rect():left(0),top(0),width(0),height(0){}
  Rect (float left, float top, float width, float height):
    left(left),top(top),width(width),height(height){}
  Rect (double left, double top, double width, double height):
    left((float)left),top((float)top),width((float)width),height((float)height){}
  Rect (int left, int top, int width, int height):
    left((float)left),top((float)top),width((float)width),height((float)height){}
  float left;
  float top;
  float width;
  float height;
};
