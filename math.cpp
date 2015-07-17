#include "math.hpp"

namespace LRR
{
  namespace Math
  {
    Matrix4f PerspectiveProjection(float fovy, float aspect, float zNear, float zFar) {
      float yScale = 1.0f / std::tan(fovy / 2);
      float xScale = yScale / aspect;
      Matrix4f pmat;
      pmat << xScale, 0, 0, 0,
              0, yScale, 0, 0,
              0, 0, -(zFar + zNear) / (zFar - zNear), -1,
              0, 0, -2 * zNear * zFar / (zFar - zNear), 0;
      return pmat;
    }

    Matrix4f RotationMatrix(AngleAxisf const &rotation) {
      auto mat3 = rotation.toRotationMatrix();
      Matrix4f mat4 = Matrix4f::Identity();
      mat4.block(0, 0, 3, 3) = mat3.block(0, 0, 3, 3);
      return mat4;
    }
  }
}