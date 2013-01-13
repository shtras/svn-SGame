#pragma once
#include "Window.h"
#include "Button.h"
#include "Input.h"

class FileOpenDialog: public Window
{
public:
  FileOpenDialog(Rect size);
  ~FileOpenDialog();
  void show();
  signal0<> Finished;
  bool getResult() {return result_;}
  CString getFileName();
  void setExtension(CString ext) {extension_ = ext;}
private:
  void fileClick(void* param);
  void clear();
  void okClick();
  void cancelClick();
  vector<Button*> buttons_;
  vector<CString> fileNames_;
  Input* input_;
  Button* ok_;
  Button* cancel_;
  bool result_;
  CString fileName_;
  CString extension_;
};