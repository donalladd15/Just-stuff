#include "quat.h"
#include <cmath>
#include <iostream>

quat::quat() : s(1.0f), v(Vector(0, 0, 0)) {}

quat::~quat() {}

quat::quat(float *M) {
  this->s = std::sqrt(1.0 + M[0] + M[5] + M[10]) / 2.0;
  float s4 = 4.0 * s;
  this->v = Vector((M[6] - M[9]) / s4, (M[8] - M[2]) / s4, (M[1] - M[4]) / s4);
}

quat::quat(float s, float x, float y, float z) {
  this->v.dx = x, this->v.dy = y, this->v.dz = z, this->s = s;
}

quat::quat(Vector v, float s) { this->v = v, this->s = s; }

quat::quat(float s, Vector v) { this->v = v, this->s = s; }

Vector quat::getImag() { return v; }

float quat::getReal() { return s; }

quat quat::mult(quat q) {
  // TODO: return Hamilton's product
  return quat((this->s * q.getReal() - this->v.dot(q.getImag())),
              (this->s * q.getImag() + q.getReal() * this->v +
               this->v.cross(q.getImag())));
}

quat quat::conjugate() {
  // TODO: return quaternion's conjugate

  return quat(this->s, -this->v.dx, -this->v.dy, -this->v.dz);
}

float quat::norm2() {
  // TODO: return square of quaternion's norm
  return (this->s * this->s + this->v.dx * this->v.dx +
          this->v.dy * this->v.dy + this->v.dz * this->v.dz);
}

quat quat::inverse() {
  // TODO: return quaternion's inverse

  return quat(this->s / norm2(), conjugate().getImag() / norm2());
}

void quat::toMatrix(float *M) {
  // M = [M[0] M[4] M[8]  M[12]
  //      M[1] M[5] M[9]  M[13]
  //      M[2] M[6] M[10] M[14]
  //      M[3] M[7] M[11] M[15]]

  // TODO update rotation coefficients i.e.
   M[0] = this->s * this->s + this->v.dx * this->v.dx - this->v.dy * this->v.dy -
           this->v.dz * this->v.dz;
   M[4] = 2.0f * (this->v.dx * this->v.dy - this->v.dz * this->s);
   M[8] = 2.0f * (this->v.dx * this->v.dz + this->v.dy * this->s);

   M[1] = 2.0f * (this->v.dx * this->v.dy + this->v.dz * this->s);
   M[5] = this->s * this->s - this->v.dx * this->v.dx + this->v.dy * this->v.dy -
           this->v.dz * this->v.dz;
   M[9] = 2.0f * (this->v.dy * this->v.dz - this->v.dx * this->s);

   M[2] = 2.0f * (this->v.dx * this->v.dz - this->v.dy * this->s);
   M[6] = 2.0f * (this->v.dy * this->v.dz + this->v.dx * this->s);
   M[10] = this->s * this->s - this->v.dx * this->v.dx -
           this->v.dy * this->v.dy + this->v.dz * this->v.dz;
}
