#pragma once

#include <memory>
#include <Eigen/Eigen>

#include "Shader.hpp"

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    struct Scanline
    {
      int min, max;
    };

    enum class ScanlineLineTarget
    {
      Min,
      Max
    };

    class ScanlineBuffer final
    {
    private:
      using ScanlineT = Scanline;
    public:
      ScanlineBuffer() = delete;
      ScanlineBuffer(int height);
      ScanlineBuffer(ScanlineBuffer const &other);
      ScanlineBuffer(ScanlineBuffer && other);

      inline int Height() const
      { return mHeight; }

      inline ScanlineT const &Scanline(int y) const {
        return mScanlines[y];
      }

      inline void Scanline(int y, ScanlineT const &scanline) {
        mScanlines[y] = scanline;
      }

      //void DrawLine(int x1, int y1, int x2, int y2, ScanlineLineTarget target);
      void DrawLine(Eigen::Vector2i const &top, Eigen::Vector2i const &bottom,
                    ScanlineLineTarget target);

      static ScanlineBuffer Rectangle(int width, int height);
      static ScanlineBuffer Triangle(Vector2i const &a, Vector2i const &b,
                                     Vector2i const &c);
    private:
      static void SortTriangleVertices(Vector2i &a, Vector2i &b,
                                       Vector2i &c);

      static float TriangleHandedness(Vector2i const &b, Vector2i const &c);

      int const mHeight;
      std::unique_ptr<ScanlineT[]> mScanlines;
    };
  }
}