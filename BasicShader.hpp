#pragma once

#include <Eigen/Eigen>

#include "Shader.hpp"
#include "math.hpp"

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    class BasicShader : public Shader
    {
    public:
      BasicShader() = delete;
      BasicShader(float aspectRatio);
      BasicShader(BasicShader const &other) = default;
      ~BasicShader() = default;

      VertexShaderOutput OnVertex() override;
      FragmentShaderOutput OnFragment() override;

      inline Matrix4f const &ModelMatrix() const
      { return mModelMatrix; }

      inline void ModelMatrix(Matrix4f const &model)
      { mModelMatrix = model; }

      inline Matrix4f const &ViewMatrix() const
      { return mViewMatrix; }

      inline void ViewMatrix(Matrix4f const &view)
      { mViewMatrix = view; }

      inline Matrix4f const &ProjectionMatrix() const
      { return mProjectionMatrix; }

      inline void ProjectionMatrix(Matrix4f const &projection)
      { mProjectionMatrix = projection; }
    private:
      Matrix4f mModelMatrix = Matrix4f::Identity();
      Matrix4f mViewMatrix = Matrix4f::Identity();
      Matrix4f mProjectionMatrix;
    };
  }
}