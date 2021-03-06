#include "ScanlineBuffer.hpp"

#include <iostream>

namespace LRR
{
  namespace Rendering
  {
    using namespace Eigen;

    ScanlineBuffer::ScanlineBuffer(int height)
      : mHeight(height),
      mScanlines(new ScanlineT[height])
    {}

    ScanlineBuffer::ScanlineBuffer(ScanlineBuffer const &other)
      : ScanlineBuffer(other.mHeight) {
      for(int i = 0; i < mHeight; ++i) {
        mScanlines[i] = other.mScanlines[i];
      }
    }

    ScanlineBuffer::ScanlineBuffer(ScanlineBuffer && other)
      : mHeight(other.mHeight),
      mScanlines(std::move(other.mScanlines))
    {}

    void ScanlineBuffer::DrawLine(Vector2i const &top, Vector2i const &bottom, ScanlineLineTarget target) {
      int const yStart = top(1);
      int const yEnd = bottom(1);
      int const xStart = top(0);
      int const xEnd = bottom(0);

      int const yDist = yEnd - yStart;
      int const xDist = xEnd - xStart;

      if(yDist <= 0) {
        return;
      }

      float xStep = float(xDist) / float(yDist);
      int topSkip = (yStart < 0 ? -yStart : 0); // TODO: Add skip for unnecessary padding between 0 and top(1)
      float x = float(xStart) + topSkip * xStep;

      // I guess this code is correct?

      for(int i = std::max(yStart, 0); i < yEnd; ++i) {
        auto index = std::min(i, mHeight - 1);

        if(target == ScanlineLineTarget::Min) {
          mScanlines[index].min = int(x);
        }
        else if(target == ScanlineLineTarget::Max) {
          mScanlines[index].max = int(x);
        }

        x += xStep;
      }
    }

    ScanlineBuffer ScanlineBuffer::Rectangle(int width, int height) {
      ScanlineBuffer slBuffer{height};
      for(int i = 0; i < height; ++i) {
        slBuffer.Scanline(i, ScanlineT{0, width});
      }
      return slBuffer;
    }

    template <typename T>
    inline void Swap(T *a, T *b) {
      T tmp = *a;
      *a = *b;
      *b = tmp;
    }
    
    void ScanlineBuffer::SortTriangleVertices(Vector2i &a, Vector2i &b,
                                              Vector2i &c) {
      // a = min Y
      // b = mid Y
      // c = max Y

      if(c(1) < b(1)) {
        Swap(&c, &b);
      }

      if(b(1) < a(1)) {
        Swap(&b, &a);
      }

      if(c(1) < b(1)) {
        Swap(&c, &b);
      }
    }

    float ScanlineBuffer::TriangleHandedness(Vector2i const &b, Vector2i const &c) {
      return float(b(0) * c(1) - c(0) * b(1));
    }

    Vector2f TriangleCentroid(Vector2f const &a, Vector2f const &b, Vector2f const &c) {
      float x = (a(0) + b(0) + c(0)) / 3.0f;
      float y = (a(1) + b(1) + c(1)) / 3.0f;
      return Vector2f{x, y};
    }

    ScanlineBuffer ScanlineBuffer::Triangle(Vector2i const &_a, Vector2i const &_b,
                                            Vector2i const &_c) {
      Vector2i a{_a};
      Vector2i b{_b};
      Vector2i c{_c};
      SortTriangleVertices(a, b, c);

      auto const height = c(1);
      ScanlineBuffer slBuffer{height};

      auto af = a.cast<float>();
      auto bf = b.cast<float>();
      auto cf = c.cast<float>();
      auto center = TriangleCentroid(af, bf, cf);

      auto handedness = TriangleHandedness((bf - center).cast<int>(), (cf - center).cast<int>());
      auto target1 = handedness >= 0 ? ScanlineLineTarget::Min : ScanlineLineTarget::Max;
      auto target2 = handedness >= 0 ? ScanlineLineTarget::Max : ScanlineLineTarget::Min;

      slBuffer.DrawLine(a, c, target1);
      slBuffer.DrawLine(a, b, target2);
      slBuffer.DrawLine(b, c, target2);

      return slBuffer;
    }
  }
}