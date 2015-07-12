#include "Rasterizer.hpp"

namespace LRR
{
  namespace Rendering
  {
    Rasterizer::Rasterizer(int frameWidth, int frameHeight)
      : mFrameBuffer(frameWidth, frameHeight)
    {}

    void Rasterizer::DrawScanlineBuffer(int x, int y, ScanlineBuffer const &slBuffer) {
      for(int i = 0; i < slBuffer.Height(); ++i) {
        auto &sl = slBuffer.Scanline(i);
        for(int r = sl.min; r < sl.max; ++r) {
          int destX = x + r;
          int destY = y + i;

          if(mFrameBuffer.IsInBounds(destX, destY)) {
            mFrameBuffer.SetPixel(destX, destY, 0xFF0000FF);
          }
        }
      }
    }
  }
}