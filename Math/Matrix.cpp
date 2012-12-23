#include "StdAfx.h"
#include "Matrix.h"

Matrix4::Matrix4()
{
  for (int i=0; i<15; ++i) {
    cont_[i] = 0;
  }
}

Matrix4::Matrix4(double* cont)
{
  for (int i=0; i<16; ++i) {
    cont_[i] = cont[i];
  }
}

Matrix4::Matrix4(double a00, double a01, double a02, double a03,
         double a10, double a11, double a12, double a13,
         double a20, double a21, double a22, double a23,
         double a30, double a31, double a32, double a33)
{
  cont_[0] = a00; cont_[1] = a01; cont_[2] = a02; cont_[3] = a03;
  cont_[4] = a10; cont_[5] = a11; cont_[6] = a12; cont_[7] = a13;
  cont_[8] = a20; cont_[9] = a21; cont_[10] = a22; cont_[11] = a23;
  cont_[12] = a30; cont_[13] = a31; cont_[14] = a32; cont_[15] = a33;
}

Matrix4::Matrix4(const Matrix4& other)
{
  for (int i=0; i<16; ++i) {
    cont_[i] = other.cont_[i];
  }
}

Matrix4::~Matrix4()
{

}

void Matrix4::loadIdentity()
{
  for (int i=0; i<16; ++i) {
    cont_[i] = 0;
  }
  cont_[0] = cont_[5] = cont_[10] = cont_[15] = 1.0f;
}

Vector4 Matrix4::getRow(const int i) const
{
  return Vector4(cont_[4*i], cont_[4*i+1], cont_[4*i+2], cont_[4*i+3]);
}

Vector4 Matrix4::getCol(const int i) const
{
  return Vector4(cont_[i], cont_[i+4], cont_[i+8], cont_[i+12]);
}

Matrix4 Matrix4::operator+ (const Matrix4& other) const
{
  Matrix4 res(*this);
  for (int i=0; i<16; ++i) {
    res.cont_[i] = cont_[i] + other.cont_[i];
  }
  return res;
}

Matrix4 Matrix4::operator- (const Matrix4& other) const
{
  Matrix4 res(*this);
  for (int i=0; i<16; ++i) {
    res.cont_[i] = cont_[i] - other.cont_[i];
  }
  return res;
}

Matrix4 Matrix4::operator* (const Matrix4& other) const
{
  Matrix4 res;
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      double sum = 0;
      for (int k=0; k<4; ++k) {
        sum += cont_[i*4+j]* other.cont_[j*4+i];
      }
      res.cont_[i*4+j] = sum;
    }
  }
  return res;
}

Matrix4 Matrix4::operator+ (const double f) const
{
  Matrix4 res(*this);
  for (int i=0; i<16; ++i) {
    res.cont_[i] = cont_[i] + f;
  }
  return res;
}

Matrix4 Matrix4::operator- (const double f) const
{
  return *this + (-f);
}

Matrix4 Matrix4::operator* (const double f) const
{
  Matrix4 res(*this);
  for (int i=0; i<16; ++i) {
    res.cont_[i] = cont_[i] * f;
  }
  return res;
}

double* Matrix4::operator[](const int i)
{
  return cont_+i*4;
}

const double* Matrix4::operator[](const int i) const
{
  return cont_+i*4;
}

double Matrix4::det() const
{
  const Matrix4& mat = *this;
  /*
  a00, a01, a02, a03,
  a10, a11, a12, a13,
  a20, a21, a22, a23,
  a30, a31, a32, a33
  */
  double d0 = mat[1][1]*(mat[2][2]*mat[3][3] - mat[2][3]*mat[3][2]) - mat[1][2]*(mat[2][1]*mat[3][3]-mat[2][3]*mat[3][1]) + mat[1][3]*(mat[2][1]*mat[3][2]-mat[2][2]-mat[3][1]);
  double d1 = mat[1][0]*(mat[2][2]*mat[3][3] - mat[2][3]*mat[3][2]) - mat[1][2]*(mat[2][0]*mat[3][3]-mat[2][3]*mat[3][0]) + mat[2][3]*(mat[2][0]*mat[3][2]-mat[2][2]*mat[3][0]);
  double d2 = mat[1][0]*(mat[2][1]*mat[3][3] - mat[2][3]*mat[3][1]) - mat[1][1]*(mat[2][0]*mat[3][3]-mat[2][3]*mat[3][0]) + mat[1][3]*(mat[2][0]*mat[3][1]-mat[2][1]*mat[3][0]);
  double d3 = mat[1][0]*(mat[2][1]*mat[3][2] - mat[2][2]*mat[3][1]) - mat[1][1]*(mat[2][0]*mat[3][2]-mat[2][2]*mat[3][0]) + mat[1][2]*(mat[2][0]*mat[3][1]-mat[2][1]*mat[3][0]);
  
  return mat[0][0]*d0 - mat[0][1]*d1 + mat[0][2]*d2 - mat[0][3]*d3;
}
