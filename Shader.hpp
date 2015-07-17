#pragma once

#include <Eigen/Eigen>

namespace LRR
{
  namespace Rendering
  {
    struct VertexShaderOutput
    {
      Eigen::Vector4f vertexPosition;
    };

    struct FragmentShaderOutput
    {
      Eigen::Vector3f fragmentColor;
    };

    class Shader
    {
    public:
      Shader() = default;
      Shader(Shader const &other) = default;
      virtual ~Shader() = default;

      virtual VertexShaderOutput OnVertex() = 0;
      virtual FragmentShaderOutput OnFragment() = 0;

      inline Eigen::Vector3f const &CurrentVertex() const
      { return mCurrentVertex; }
      
      inline void CurrentVertex(Eigen::Vector3f const &vertex)
      { mCurrentVertex = vertex; }
    protected:
      Eigen::Vector3f mCurrentVertex;
    };
  }
}