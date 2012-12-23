#include "StdAfx.h"
#include "Vector.h"
#include "Matrix.h"
#include <math.h>

Vector3::Vector3()
{
  cont_[0] = 0; cont_[1] = 0; cont_[2] = 0;
}

Vector3::Vector3(double* cont)
{
  cont_[0] = cont[0]; cont_[1] = cont[1]; cont_[2] = cont[2];
}

Vector3::Vector3(double x, double y, double z)
{
  cont_[0] = x; cont_[1] = y; cont_[2] = z;
}

Vector3::Vector3(const Vector3& other)
{
  cont_[0] = other.cont_[0]; cont_[1] = other.cont_[1]; cont_[2] = other.cont_[2];
}

Vector3::~Vector3()
{

}

void Vector3::normalize()
{
  double div = sqrt(cont_[0]*cont_[0] + cont_[1]*cont_[1] + cont_[2]*cont_[2]);
  if (div == 0) {
    return;
  }
  cont_[0] /= div; cont_[1] /= div; cont_[2] /= div;
}

Vector3 Vector3::getNormalized() const
{
  Vector3 res(*this);
  res.normalize();
  return res;
}

double Vector3::dot(const Vector3& other) const
{
  return (cont_[0]*other.cont_[0] + cont_[1]*other.cont_[1] + cont_[2]*other.cont_[2]);
}

double Vector3::getLength()
{
  return sqrt(cont_[0]*cont_[0] + cont_[1]*cont_[1] + cont_[2]*cont_[2]);
}

Vector3 Vector3::operator+ (const Vector3& other) const
{
  return Vector3(cont_[0] + other.cont_[0], cont_[1] + other.cont_[1], cont_[2] + other.cont_[2]);
}

Vector3 Vector3::operator* (const Vector3& other) const
{
  /*
  |x^ y^ z^|
  |x1 y1 z1| = x^(y1*z2-z1*y2) - y^(x1*z2-z1*x2) + z^(x1*y2-y1*x2)
  |x2 y2 z2|
  */
  double x = cont_[1]*other.cont_[2] - cont_[2]*other.cont_[1];
  double y = -(cont_[0]*other.cont_[2] - cont_[2]*other.cont_[0]);
  double z = cont_[0]*other.cont_[1] - cont_[1]*other.cont_[0];
  return Vector3(x,y,z);
}

Vector3 Vector3::operator- (const Vector3& other) const
{
  return Vector3(cont_[0] - other.cont_[0], cont_[1] - other.cont_[1], cont_[2] - other.cont_[2]);
}

Vector3 Vector3::operator+ (const double f) const
{
  return Vector3(cont_[0] + f, cont_[1] + f, cont_[2] + f);
}

Vector3 Vector3::operator* (const double f) const
{
  return Vector3(cont_[0] * f, cont_[1] * f, cont_[2] * f);
}

Vector3 Vector3::operator- (const double f) const
{
  return Vector3(cont_[0] - f, cont_[1] - f, cont_[2] - f);
}

double Vector3::getAngle()
{
  if (fabs(cont_[0]) < 0.00001) {
    return (cont_[1] < 0)?0:180;
  }
  //double c = cont_[0]/cont_[1];
  double ra = atan2(cont_[0], -cont_[1]);
  double res = RadToDeg(ra);
  if (res < 180.0) {
    res += 360.0;
  }
  if (res > 180.0) {
    res -= 360.0;
  }
  assert(res >= -180.0 && res <= 180.0);
  return res;
  //double da = -RadToDeg(ra) + 90.0;
  //if (cont_[0] > 0 && cont_[1] > 0) {
  //  return da;
  //}
  //if (cont_[0] < 0 && cont_[1] > 0) {
  //  return 90.0 - da;
  //}
  //if (cont_[0] < 0 && cont_[1] < 0) {
  //  return 90.0 + da;
  //}
  //return 180.0 - da;
}

//Vector4
Vector4::Vector4()
{
  cont_[0] = 0; cont_[1] = 0; cont_[2] = 0; cont_[3] = 0;
}

Vector4::Vector4(double* cont)
{
  cont_[0] = cont[0]; cont_[1] = cont[1]; cont_[2] = cont[2]; cont_[3] = cont[3];
}

Vector4::Vector4(double x, double y, double z, double w)
{
  cont_[0] = x; cont_[1] = y; cont_[2] = z; cont_[3] = w;
}

Vector4::Vector4(const Vector4& other)
{
  cont_[0] = other.cont_[0]; cont_[1] = other.cont_[1]; cont_[2] = other.cont_[2]; cont_[3] = other.cont_[3];
}

Vector4::~Vector4()
{

}

void Vector4::normalize()
{
  double div = sqrt(cont_[0]*cont_[0] + cont_[1]*cont_[1] + cont_[2]*cont_[2] + cont_[3]*cont_[3]);
  cont_[0] /= div; cont_[1] /= div; cont_[2] /= div; cont_[3] /= div;
}

Vector4 Vector4::getNormalized() const
{
  Vector4 res(*this);
  res.normalize();
  return res;
}

double Vector4::dot(const Vector4& other) const
{
  return (cont_[0]*other.cont_[0] + cont_[1]*other.cont_[1] + cont_[2]*other.cont_[2] + cont_[3]*cont_[3]);
}

Vector4 Vector4::operator+ (const Vector4& other) const
{
  return Vector4(cont_[0] + other.cont_[0], cont_[1] + other.cont_[1], cont_[2] + other.cont_[2], cont_[3] + other.cont_[3]);
}

Vector4 Vector4::operator- (const Vector4& other) const
{
  return Vector4(cont_[0] - other.cont_[0], cont_[1] - other.cont_[1], cont_[2] - other.cont_[2], cont_[3] - other.cont_[3]);
}

Vector4 Vector4::operator+ (const double f) const
{
  return Vector4(cont_[0] + f, cont_[1] + f, cont_[2] + f, cont_[3] + f);
}

Vector4 Vector4::operator* (const double f) const
{
  return Vector4(cont_[0] * f, cont_[1] * f, cont_[2] * f, cont_[3] * f);
}

Vector4 Vector4::operator- (const double f) const
{
  return Vector4(cont_[0] - f, cont_[1] - f, cont_[2] - f, cont_[3] * f);
}

Vector4 Vector4::operator* (const Matrix4& mat) const
{
  Vector4 res;
  for (int i=0; i<4; ++i) {
    double sum = 0;
    for (int j=0; j<4; ++j) {
      sum += cont_[j] * mat[j][i];
    }
    res[i] = sum;
  }
  return res;
}
