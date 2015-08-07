#pragma once

#include "Bitmap.hpp"
#include "ScanlineBuffer.hpp"
#include "Shader.hpp"

#include <Eigen/StdVector>
#include <vector>

template <typename T>
using EigenVec = std::vector<T, Eigen::aligned_allocator<T>>;

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    class Rasterizer final
    {
    private:
      using BitmapT = Rendering::Bitmap;
    public:
      Rasterizer() = delete;
      Rasterizer(int frameWidth, int frameHeight);
      ~Rasterizer() = default;

      inline BitmapT &Bitmap()
      { return mFrameBuffer; }

      inline void Clear(uint32_t color) {
        mFrameBuffer.Clear(color);
      }

      void DrawTriangle(Vector3f const &a, Vector3f const &b, Vector3f const &c, Shader &shader);
    private:
      void DrawScanlineBuffer(int x, int y, ScanlineBuffer const &slBuffer, Shader &shader);
      static void ClipPolygonComponent(EigenVec<Vector4f> const &vertices, EigenVec<Vector4f> &clippedOut, unsigned componentIndex, float factor);
      static bool ClipPolygonAxis(EigenVec<Vector4f> &vertices, EigenVec<Vector4f> &auxList, unsigned componentIndex);
      void FillTriangle(Vector3f const &a, Vector3f const &b, Vector3f const &c, Shader &shader);

      BitmapT mFrameBuffer;
    };
  }
}