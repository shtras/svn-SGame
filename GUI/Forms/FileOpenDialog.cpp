#include "StdAfx.h"
#include "FileOpenDialog.h"
#include <dirent.h>

FileOpenDialog::FileOpenDialog( Rect size ):Window(size)
{
  visible_ = false;
  input_ = new Input(Rect(0.01, 0.95, 0.5, 0.04));
  addWidget(input_);

  ok_ = new Button(Rect(0.51, 0.95, 0.24, 0.04));
  ok_->setCaption("Ok");
  ok_->setColor(Vector4(255, 0, 0, 255));
  addWidget(ok_);
  ok_->sigClick.connect(this, &FileOpenDialog::okClick);

  cancel_ = new Button(Rect(0.75, 0.95, 0.24, 0.04));
  cancel_->setCaption("Cancel");
  cancel_->setColor(Vector4(255, 0, 0, 255));
  addWidget(cancel_);
  cancel_->sigClick.connect(this, &FileOpenDialog::cancelClick);
}

FileOpenDialog::~FileOpenDialog()
{

}

void FileOpenDialog::show()
{
  clear();
  DIR *dp;
  struct dirent *dirp;
  if((dp = opendir("saves")) == NULL) {
    return;
  }
  int i=0;
  float top = 0.01f;
  while ((dirp = readdir(dp)) != NULL) {
    CString fileName = dirp->d_name;
    if (fileName == "." || fileName == "..") {
      continue;
    }
    vector<CString> parts = fileName.tokenize('.');
    if (extension_ != "" && parts.back() != extension_) {
      continue;
    }
    fileName = "";
    for (int j=0; j<(int)parts.size()-1; ++j) {
      fileName += parts[j];
      if (j != parts.size()-2) {
        fileName += ".";
      }
    }
    Button* b = new Button(Rect(0.01f, top, 0.98f, 0.07f));
    b->setCaption(fileName);
    b->setParam((void*)i);
    b->sig1Click.connect(this, &FileOpenDialog::fileClick);
    addWidget(b);
    b->setColor(Vector4(255, 0, 0, 255));
    buttons_.push_back(b);
    fileNames_.push_back(fileName);
    ++i;
    top += 0.07f;
  }
  closedir(dp);

  visible_ = true;
}

void FileOpenDialog::clear()
{
  assert (buttons_.size() == fileNames_.size());
  for (int i=0; i<(int)buttons_.size(); ++i) {
    removeWidget(buttons_[i]);
    delete buttons_[i];
  }
  buttons_.clear();
  fileNames_.clear();
  input_->setCaption("");
}

void FileOpenDialog::fileClick(void* param)
{
  int i = (int)param;
  CString fileName = fileNames_[i];
  input_->setCaption(fileName);
}

void FileOpenDialog::okClick()
{
  visible_ = false;
  result_ = true;
  fileName_ = input_->getCaption();
  Finished.emit();
}

void FileOpenDialog::cancelClick()
{
  visible_ = false;
  result_ = false;
  Finished.emit();
}

CString FileOpenDialog::getFileName()
{
  if (extension_ == "") {
    return fileName_;
  }
  return fileName_ + "." + extension_;
}
