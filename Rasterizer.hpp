#pragma once

#include "Bitmap.hpp"
#include "ScanlineBuffer.hpp"

namespace LRR
{
  namespace Rendering
  {
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

      void DrawScanlineBuffer(int x, int y, ScanlineBuffer const &slBuffer);
    private:
      BitmapT mFrameBuffer;
    };
  }
}