#include "BasicShader.hpp"

#include <SDL.h>

#include "math.hpp"

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
      auto mvp = mProjectionMatrix * mViewMatrix * mModelMatrix;
      return {mvp * v4};
    }

    FragmentShaderOutput BasicShader::OnFragment() {
      float time = SDL_GetTicks() / 1000.0f;

      float r = mFragCoord(0) + 1 / 2.0f;
      float g = (std::sin(Math::ToRadians(time * 180)) + 1 / 2.0f) * 0.3f;
      float b = mFragCoord(1) + 1 / 2.0f;

      return {{r, g, b}};
    }
  }
}