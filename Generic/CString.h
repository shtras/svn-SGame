#pragma once
#include <string>
using namespace std;

class CString
{
  friend CString operator+ (const char* s1, const CString& s2);
public:
  CString();
  CString(const CString& other);
  CString(CString&& other);
  CString(const char* str);
  CString(char c);
  CString(int val);
  CString(int val, int minLen);
  CString(float val);
  CString(float val, int len);
  CString(double val);
  CString(double val, int len);
  CString(long val);
  ~CString();

  CString left(int idx);
  int getSize() const;
  CString append(const CString& other) const;
  int getIndex(char c) const;
  bool contains(CString other) const;
  bool contains(char c) const;
  operator const char*() const;
  CString operator+ (const CString& other) const;
  CString& operator= (CString other);
  CString& operator= (const char* str);
  bool operator== (const CString& other) const;
  bool operator== (const char* other) const;
  bool operator!= (const CString& other) const;
  bool operator!= (const char* other) const;
  bool operator< (const CString& other) const;
  CString& operator+= (const CString& other);
  bool isEmpty();
  CString toLower();
  CString toUpper();
  CString remove(char c);
  CString substr(int begin, int end);
  vector<CString> tokenize(char c);
  void replace(char c, char to);
  int hashCode();
private:
  char* cont_;
  int len_;

  //string str_;
};

CString operator+ (const char* s1, const CString& s2);
