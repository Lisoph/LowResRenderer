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
      for(int i = std::max(topY, 0); i < bottomY; ++i, x += stepX) {
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

      auto const height = c(1) - a(1) + a(1);
      ScanlineBuffer slBuffer{height};

      auto af = a.cast<float>();
      auto bf = b.cast<float>();
      auto cf = c.cast<float>();
      auto center = TriangleCentroid(af, bf, cf);

      auto handedness = TriangleHandedness((bf - center).cast<int>(), (cf - center).cast<int>());
      auto target1 = handedness >= 0 ? ScanlineLineTarget::Min : ScanlineLineTarget::Max;
      auto target2 = handedness >= 0 ? ScanlineLineTarget::Max : ScanlineLineTarget::Min;

      slBuffer.DrawLine(a(0), a(1), c(0), c(1), target1);
      slBuffer.DrawLine(a(0), a(1), b(0), b(1), target2);
      slBuffer.DrawLine(b(0), b(1), c(0), c(1), target2);

      return slBuffer;
    }
  }
}