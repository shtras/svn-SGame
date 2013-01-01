#include "StdAfx.h"
#include "CString.h"
#include <sstream>
#include <iomanip>

CString::CString()
{
  cont_ = new char[1];
  cont_[0] = '\0';
  len_ = 0;
  //str_ = "";
}

CString::CString(const CString& other)//:str_(other)
{
  cont_ = new char[other.len_+1];
  strcpy(cont_, other.cont_);
  len_ = other.len_;
}

CString::CString(const char* str)//:str_(str)
{
  len_ = strlen(str);
  cont_ = new char[len_+1];
  strcpy(cont_, str);
}

CString::CString(char c)
{
  len_ = 1;
  cont_ = new char[2];
  cont_[0] = c;
  cont_[1] = '\0';
}

CString::CString(int val)
{
  int origval = val;
  int len = (val > 0)? 0:1;
  while (val != 0) {
    val /= 10;
    len++;
  }
  cont_ = new char[len+1];
  _itoa(origval, cont_, 10);
  len_ = len;

  //std::stringstream ss;
  //ss << val;
  //str_ = ss.str();
}

CString::CString(int val, int minLen)
{
  int origval = val;
  int len = (val > 0)? 0:1;
  while (val != 0) {
    val /= 10;
    len++;
  }
  if (len < minLen) {
    len = minLen;
  }
  cont_ = new char[len+1];
  CString format = "%0" + CString(minLen) + "d";
  sprintf(cont_, format, origval);
  //_itoa(origval, cont_, 10);
  len_ = len;
}

CString::CString(float val)
{
  std::stringstream ss;
  ss << val;
  std::string str = ss.str();
  len_ = str.size();
  cont_ = new char[len_+1];
  strcpy(cont_, str.c_str());
}

CString::CString( float val, int len )
{
  std::stringstream ss;
  ss << setprecision(len) << setiosflags(ios_base::fixed) << val;
  std::string str = ss.str();
  len_ = str.size();
  cont_ = new char[len_+1];
  strcpy(cont_, str.c_str());
}

CString::CString(double val)
{
  std::stringstream ss;
  ss << val;
  std::string str = ss.str();
  len_ = str.size();
  cont_ = new char[len_+1];
  strcpy(cont_, str.c_str());

  //str_ = str;
}

CString::CString(double val, int len)
{
  std::stringstream ss;
  ss << setprecision(len) << setiosflags(ios_base::fixed) << val;
  std::string str = ss.str();
  len_ = str.size();
  cont_ = new char[len_+1];
  strcpy(cont_, str.c_str());

  //str_ = str;
}

CString::CString(long val)
{

}

CString::~CString()
{
  delete[] cont_;
  cont_ = NULL;
}

int CString::getSize() const
{
  //return str_.length();
  return len_;
}

CString CString::left(int idx)
{
  assert(idx<len_ && idx>=0);
  CString res;
  delete[] res.cont_;
  res.cont_ = new char[idx+1];
  for (int i=0; i<idx; ++i) {
    res.cont_[i] = cont_[i];
  }
  res.cont_[idx] = '\0';
  res.len_ = idx;
  return res;
}

CString CString::append(const CString& other) const
{
  CString res;
  delete[] res.cont_;
  res.cont_ = new char[len_ + other.len_ + 1];
  strcpy(res.cont_, cont_);
  strcpy(res.cont_ + len_, other.cont_);
  res.len_ = len_ + other.len_;
  return res;
}

int CString::getIndex(char c) const
{
  for (int i=0; i<len_; ++i) {
    if (cont_[i] == c) {
      return i;
    }
  }
  return -1;
}

bool CString::contains(CString other) const
{
  if (other.getSize() > getSize()) {
    return false;
  }
  if (other.getSize() == getSize()) {
    return *this == other;
  }
  int cnt = 0;
  bool seq = false;
  for (int i=0; i<len_; ++i) {
    if (cont_[i] == other[cnt]) {
      ++cnt;
      if (seq) {
        if (cnt == other.getSize() - 1) {
          return true;
        }
      } else {
        seq = true;
      }
    } else {
      cnt = 0;
      seq = false;
    }
  }
  return false;
}

bool CString::contains(char c) const
{
  for (int i=0; i<len_; ++i) {
    if (cont_[i] == c) {
      return true;
    }
  }
  return false;
}

CString::operator const char*() const
{
  return cont_;
}

CString CString::operator+ (const CString& other) const
{
  return this->append(other);
}

CString& CString::operator= (CString& other)
{
  delete[] cont_;
  cont_ = new char[other.len_+1];
  strcpy(cont_, other.cont_);
  len_ = other.len_;
  return *this;
}

CString& CString::operator= (const char* str)
{
  delete[] cont_;
  len_ = strlen(str);
  cont_ = new char[len_+1];
  strcpy(cont_, str);
  return *this;
}

bool CString::operator== (const CString& other) const
{
  if (other.len_ != len_) {
    return false;
  }
  return (strcmp(cont_, other.cont_) == 0);
}

bool CString::operator== (const char* other) const
{
  return *this == CString(other);
}

bool CString::operator!= (const CString& other) const
{
  return !(*this == other);
}

bool CString::operator!= (const char* other) const
{
  return !(*this == other);
}

CString& CString::operator+= (const CString& other)
{
  *this = append(other);
  return *this;
}

bool CString::isEmpty()
{
  return len_ == 0;
}

bool CString::operator< (const CString& other) const
{
  return strcmp(cont_, other.cont_) < 0;
}

CString CString::remove(char c)
{
  CString res = "";
  for (int i=0; i<len_; ++i) {
    if (cont_[i] != c) {
      res = res.append(cont_[i]);
    }
  }
  return res;
}

vector<CString> CString::tokenize(char c)
{
  vector<CString> res;
  CString currString;
  for (int i=0; i<len_; ++i) {
    if (cont_[i] != c) {
      currString = currString.append(cont_[i]);
    } else {
      res.push_back(currString);
      currString = "";
    }
  }
  if (!currString.isEmpty()) {
    res.push_back(currString);
  }
  return res;
}

CString CString::toLower()
{
  CString res = CString(*this);
  for (int i=0; i<res.len_; ++i) {
    if (res.cont_[i] >= 'A' && res.cont_[i] <= 'Z') {
      res.cont_[i] = res.cont_[i] - 'A' + 'a';
    }
  }
  return res;
}

CString CString::toUpper()
{
  CString res = CString(*this);
  for (int i=0; i<res.len_; ++i) {
    if (res.cont_[i] >= 'a' && res.cont_[i] <= 'a') {
      res.cont_[i] = res.cont_[i] - 'a' + 'A';
    }
  }
  return res;
}

CString CString::substr(int begin, int end)
{
  if (begin < 0 || begin >= len_ || end < 0 || end >= len_) {
    return "";
  }
  CString res;
  for (int i=begin; i<=end; ++i) {
    res = res.append(cont_[i]);
  }
  return res;
}

void CString::replace(char c, char to)
{
  for (int i=0; i<len_; ++i) {
    if (cont_[i] == c) {
      cont_[i] = to;
    }
  }
}

CString operator+ (const char* s1, const CString& s2)
{
  return CString(s1) + s2;
}
