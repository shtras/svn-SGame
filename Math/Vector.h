/**
 * Linear algebra vectors
 * Author: Gregory Shtrasberg
 * Date: 01.2010
 **/
#pragma once

class Matrix4;

class Vector3
{
public:
  Vector3();
  Vector3(double* cont);
  Vector3(double x, double y, double z);
  Vector3(const Vector3& other);
  ~Vector3();

  void normalize();
  Vector3 getNormalized() const;
  double dot(const Vector3& other) const;
  double getLength();
  Vector3 operator+ (const Vector3& other) const;
  Vector3 operator* (const Vector3& other) const;
  Vector3 operator- (const Vector3& other) const;
  Vector3 operator+ (const double f) const;
  Vector3 operator* (const double f) const;
  Vector3 operator- (const double f) const;
  Vector3& operator+= (const Vector3& other) {return *this = *this + other;}
  Vector3& operator*= (const Vector3& other) {return *this = *this * other;}
  Vector3& operator-= (const Vector3& other) {return *this = *this - other;}
  Vector3& operator+= (const double f) {return *this = *this + f;}
  Vector3& operator*= (const double f) {return *this = *this * f;}
  Vector3& operator-= (const double f) {return *this = *this - f;}

  double& operator[] (const int i) {assert (i>=0 && i<3); return cont_[i];}
  const double operator[] (const int i) const {assert (i>=0 && i<3); return cont_[i];}
  operator double*() {return cont_;}
  double getAngle();
protected:
  double cont_[3];
};

class Vector4
{
public:
  Vector4();
  Vector4(double* cont);
  Vector4(double x, double y, double z, double w);
  Vector4(const Vector4& other);
  ~Vector4();

  void normalize();
  Vector4 getNormalized() const;
  double dot(const Vector4& other) const;
  Vector4 operator+ (const Vector4& other) const;
  Vector4 operator- (const Vector4& other) const;
  Vector4 operator+ (const double f) const;
  Vector4 operator* (const double f) const;
  Vector4 operator- (const double f) const;
  Vector4& operator+= (const Vector4& other) {return *this = *this + other;}
  Vector4& operator-= (const Vector4& other) {return *this = *this - other;}
  Vector4& operator+= (const double f) {return *this = *this + f;}
  Vector4& operator*= (const double f) {return *this = *this * f;}
  Vector4& operator-= (const double f) {return *this = *this - f;}

  Vector4 operator* (const Matrix4& mat) const;

  double& operator[] (const int i) {assert (i>=0 && i<4); return cont_[i];}
  const double operator[] (const int i) const {assert (i>=0 && i<4); return cont_[i];}
  operator double*() {return cont_;}
private:
  double cont_[4];
};
