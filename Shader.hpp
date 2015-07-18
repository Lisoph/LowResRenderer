#pragma once

#include <Eigen/Eigen>

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    struct VertexShaderOutput
    {
      Vector4f vertexPosition;
    };

    struct FragmentShaderOutput
    {
      Vector3f fragmentColor;
    };

    class Shader
    {
    public:
      Shader() = default;
      Shader(Shader const &other) = default;
      virtual ~Shader() = default;

      virtual VertexShaderOutput OnVertex() = 0;
      virtual FragmentShaderOutput OnFragment() = 0;

      inline Vector3f const &CurrentVertex() const
      { return mCurrentVertex; }
      
      inline void CurrentVertex(Vector3f const &vertex)
      { mCurrentVertex = vertex; }

      inline Vector2f const &FragCoord() const
      { return mFragCoord; }

      inline void FragCoord(Vector2f const &fragCoord)
      { mFragCoord = fragCoord; }
    protected:
      Vector3f mCurrentVertex;
      Vector2f mFragCoord;
    };
  }
}