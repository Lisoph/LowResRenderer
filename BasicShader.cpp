#include "BasicShader.hpp"

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    BasicShader::BasicShader(float aspectRatio) {
      mProjectionMatrix = Math::PerspectiveProjection(Math::ToRadians(90.0f), aspectRatio, 0.1f, 1000.0f);
    }

    VertexShaderOutput BasicShader::OnVertex() {
      Vector4f v4{mCurrentVertex(0), mCurrentVertex(1), mCurrentVertex(2), 1.0f};
      //auto mvp = mModelMatrix * mViewMatrix * mProjectionMatrix;
      //auto mvp = mProjectionMatrix * mViewMatrix * mModelMatrix;
      return {mProjectionMatrix * mViewMatrix * mModelMatrix * v4};
    }

    FragmentShaderOutput BasicShader::OnFragment() {
      return {{1, 0, 0}};
    }
  }
}