#include "QDebug"
#include "arcball.h"
#include <cmath>

ArcBall::ArcBall() {
  q = quat();
  v = Vector();
}

ArcBall::~ArcBall() {}

// Converts screen coordinates to a point on a unit sphere
// Get a normalized vector u from the center O of the virtual ball to
// a point P on the virtual ball surface, such that P is aligned on
// the screen's (x, y) coordinates.
Vector ArcBall::screenToPoint(int x, int y, int width, int height) {
  Vector u = Vector(2. * x / width - 1., 1. - 2. * y / height, 0.);
  float d = u.dot(u);
  if (d <= 1) {
    // If (x, y) is too far away from the sphere, return the nearest
    // point on the virtual ball surface.
    u.dz = sqrt(1 - d);
  } else {
    u = u.normalised();
  }
  return u;
}

void ArcBall::startBall(int x, int y, int width, int height) {
  v = screenToPoint(x, y, width, height);
}

// Use technique in:
// ARCBALL: A User Interface for Specifying
// Three-Dimensional Orientation Using a Mouse.
// K Shoemake. Graphics interface '92, pages 151-156, 1992.
void ArcBall::updateBall(int x, int y, int width, int height) {
  Vector u = screenToPoint(x, y, width, height);
  // TODO: update arcball quaternion
  quat q_now = quat(v.dot(u), v.cross(u));
  q = q_now;

}

// Replace modelview matrix with rotation coefficients from quaternion q
void ArcBall::getMatrix(float *M) { this->q.toMatrix(M); }

void ArcBall::reset() { q = quat(); }
