#pragma once

#include "Bitmap.hpp"
#include "ScanlineBuffer.hpp"
#include "Shader.hpp"

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

      BitmapT mFrameBuffer;
    };
  }
}