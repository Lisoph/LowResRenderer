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

      /*float radf = Math::ToRadians(fovy);
      float tanHalfFovy = std::tan(radf / 2.0f);

      Matrix4f res = Matrix4f::Identity();
      res(0, 0) = 1.0f / (aspect * tanHalfFovy);
      res(1, 1) = 1.0f / (tanHalfFovy);
      res(2, 2) = -(zFar + zNear) / (zFar - zNear);
      res(3, 2) = -1.0f;
      res(2, 3) = -(2.0f * zFar * zNear) / (zFar - zNear);
      return res;*/

      /*float theta = fovy * 0.5f;
      float range = zFar - zNear;
      float invtan = 1.0f / std::tan(theta);

      Matrix4f mat;
      mat(0, 0) = invtan / aspect;
      mat(1, 1) = invtan;
      mat(2, 2) = -(zNear + zFar) / range;
      mat(3, 2) = -1;
      mat(2, 3) = -2 * zNear * zFar / range;
      mat(3, 3) = 0;
      return mat;*/
    }

    Matrix4f RotationMatrix(AngleAxisf const &rotation) {
      auto mat3 = rotation.toRotationMatrix();
      Matrix4f mat4 = Matrix4f::Identity();
      mat4.block(0, 0, 3, 3) = mat3.block(0, 0, 3, 3);
      return mat4;
    }
  }
}