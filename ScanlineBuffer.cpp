#include "ScanlineBuffer.hpp"

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

    inline float Sign(float x) {
      return x >= 0.0f ? 1.0f : -1.0f;
    }

    void ScanlineBuffer::DrawLine(int x1, int y1, int x2, int y2, ScanlineLineTarget target) {
      int const height = std::abs(y1 - y2);
      int const topY = std::min(y1, y2);
      int const bottomY = std::max(y1, y2);
      int const leftX = std::min(x1, x2);
      int const rightX = std::max(x1, x2);

      float const distX = float(rightX - leftX);
      float const distY = float(bottomY - topY);
      float const stepX = distX / distY * Sign(float(x2 - x1));

      float x = float(x1);
      for(int i = topY; i < bottomY && i < mHeight; ++i, x += stepX) {
        if(target == ScanlineLineTarget::Min) {
          mScanlines[i].min = int(x);
        }
        else if(target == ScanlineLineTarget::Max) {
          mScanlines[i].max = int(x);
        }
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
        /*auto tmp = b;
        b = c;
        c = tmp;*/
        Swap(&c, &b);
      }

      if(b(1) < a(1)) {
        /*auto tmp = a;
        a = b;
        b = tmp;*/
        Swap(&b, &a);
      }

      if(c(1) < b(1)) {
        /*auto tmp = b;
        b = c;
        c = tmp;*/
        Swap(&c, &b);
      }
    }

    float ScanlineBuffer::TriangleHandedness(Vector2i const &b, Vector2i const &c) {
      float const x1 = b(0);
      float const y1 = b(1);

      float const x2 = c(0);
      float const y2 = c(1);

      //return float(b(0) * c(1) - c(0) * b(1));
      return x1 * y2 - x2 * y1;
    }

    ScanlineBuffer ScanlineBuffer::Triangle(Vector2i const &_a, Vector2i const &_b,
                                            Vector2i const &_c) {
      Vector2i a{_a};
      Vector2i b{_b};
      Vector2i c{_c};
      SortTriangleVertices(a, b, c);

      auto const height = c(1) - a(1) + a(1);
      //auto const height = 1000;
      ScanlineBuffer slBuffer{height};

      auto handedness = TriangleHandedness(b, c);
      auto target1 = handedness >= 0 ? ScanlineLineTarget::Min : ScanlineLineTarget::Max;
      auto target2 = handedness >= 0 ? ScanlineLineTarget::Max : ScanlineLineTarget::Min;

      slBuffer.DrawLine(a(0), a(1), c(0), c(1), target1);
      slBuffer.DrawLine(a(0), a(1), b(0), b(1), target2);
      slBuffer.DrawLine(b(0), b(1), c(0), c(1), target2);

      return slBuffer;
    }
  }
}