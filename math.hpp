#pragma once

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>

namespace LRR
{
  namespace Math
  {
    using namespace Eigen;

    template <typename T>
    T ToRadians(T deg) { return deg * T(0.01745329251f); }

    template <typename T>
    T ToDegrees(T rad) { return rad * T(57.2957795131f); }

    Matrix4f PerspectiveProjection(float fovy, float aspect, float zNear, float zFar);

    Matrix4f RotationMatrix(AngleAxisf const &rotation);
  }
}