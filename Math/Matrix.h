/**
 * Linear algebra matrices
 * Author: Gregory Shtrasberg
 * Date: 01.2010
 **/
#pragma once
#include "Vector.h"

class Matrix4
{
public:
  Matrix4();
  Matrix4(double* cont);
  Matrix4(double a00, double a01, double a02, double a03,
           double a10, double a11, double a12, double a13,
           double a20, double a21, double a22, double a23,
           double a30, double a31, double a32, double a33);
  Matrix4(const Matrix4& other);
  ~Matrix4();

  void loadIdentity();

  Vector4 getRow(const int i) const;
  Vector4 getCol(const int i) const;

  Matrix4 operator+ (const Matrix4& other) const;
  Matrix4 operator- (const Matrix4& other) const;
  Matrix4 operator* (const Matrix4& other) const;

  Matrix4 operator+ (const double f) const;
  Matrix4 operator- (const double f) const;
  Matrix4 operator* (const double f) const;

  Matrix4& operator+= (const Matrix4& other) {return *this = *this + other;}
  Matrix4& operator-= (const Matrix4& other) {return *this = *this - other;}
  Matrix4& operator*= (const Matrix4& other) {return *this = *this * other;}

  Matrix4& operator+= (const double f) {return *this = *this + f;}
  Matrix4& operator-= (const double f) {return *this = *this - f;}
  Matrix4& operator*= (const double f) {return *this = *this * f;}

  double* operator[](const int i);
  const double* operator[](const int i) const;

  double det() const;
  operator double*() {return cont_;}
private:
  double cont_[16];
};
