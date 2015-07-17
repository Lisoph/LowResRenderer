#include "Rasterizer.hpp"

namespace LRR
{
  namespace Rendering
  {
    Rasterizer::Rasterizer(int frameWidth, int frameHeight)
      : mFrameBuffer(frameWidth, frameHeight)
    {}

    void Rasterizer::DrawScanlineBuffer(int x, int y, ScanlineBuffer const &slBuffer, Shader &shader) {
      for(int i = 0; i < std::min(slBuffer.Height(), mFrameBuffer.Height()); ++i) {
        int destY = y + i;
        if(destY < 0 || destY >= mFrameBuffer.Height()) continue;

        auto &sl = slBuffer.Scanline(i);
        for(int r = sl.min; r < sl.max; ++r) {
          int destX = x + r;

          if(destX >= 0 && destX < mFrameBuffer.Width()) {
            auto fragColor = shader.OnFragment().fragmentColor;
            auto red = uint32_t(fragColor(0) * 255);
            auto green = uint32_t(fragColor(1) * 255);
            auto blue = uint32_t(fragColor(2) * 255);
            mFrameBuffer.SetPixel(destX, destY, (red << 24) | (green << 16) | (blue << 8) | 0xFF);
          }
        }
      }
    }

    void Rasterizer::DrawTriangle(Vector3f const &a, Vector3f const &b, Vector3f const &c, Shader &shader) {
      shader.CurrentVertex(a);
      auto pos1 = shader.OnVertex().vertexPosition;
      shader.CurrentVertex(b);
      auto pos2 = shader.OnVertex().vertexPosition;
      shader.CurrentVertex(c);
      auto pos3 = shader.OnVertex().vertexPosition;

      // Perspective division
      static auto divider = [](Vector4f const &vertex) -> Vector4f {
        float div = (vertex(3) == 0 ? 1 : vertex(3));
        return vertex / div;
      };

      pos1 = divider(pos1);
      pos2 = divider(pos2);
      pos3 = divider(pos3);

      // Map to device coordinates
      static auto mapper = [&](Vector4f const &ndc) -> Vector2f {
        float width = float(mFrameBuffer.Width());
        float height = float(mFrameBuffer.Height());
        float x = (ndc(0) * 0.5f + 0.5f) * width;
        float y = (-ndc(1) * 0.5f + 0.5f) * height;
        return {x, y};
      };

      auto dev1 = mapper(pos1);
      auto dev2 = mapper(pos2);
      auto dev3 = mapper(pos3);

      auto slb = ScanlineBuffer::Triangle(dev1.cast<int>(), dev2.cast<int>(), dev3.cast<int>());
      DrawScanlineBuffer(0, 0, slb, shader);
    }
  }
}