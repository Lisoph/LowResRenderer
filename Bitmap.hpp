#pragma once

#include <memory>
#include <cstdlib>

namespace LRR
{
  namespace Rendering
  {
    class Bitmap final
    {
    public:
      Bitmap() = delete;
      Bitmap(int width, int height);
      Bitmap(Bitmap const &other);
      Bitmap(Bitmap && other);
      ~Bitmap() = default;

      inline int Width() const
      { return mWidth; }
      
      inline int Height() const
      { return mHeight; }

      inline bool IsInBounds(int i) const {
        return i > 0 && i < mWidth * mHeight;
      }

      inline bool IsInBounds(int x, int y) const {
        return IsInBounds(mWidth * y + x);
      }

      inline void SetPixel(int i, uint32_t pixel) {
        mPixels[i] = pixel;
      }

      inline void SetPixel(int x, int y, uint32_t pixel) {
        SetPixel(mWidth * y + x, pixel);
      }

      inline uint32_t GetPixel(int i) {
        return mPixels[i];
      }

      inline uint32_t GetPixel(int x, int y) {
        return mPixels[mWidth * y + x];
      }

      inline void Clear(uint32_t color) {
        auto const max = mWidth * mHeight;
        for(int i = 0; i < max; ++i) {
          SetPixel(i, color);
        }
      }

      inline void ClearRandom(uint32_t shiftBits) {
        auto const max = mWidth * mHeight;
        for(int i = 0; i < max; ++i) {
          SetPixel(i, rand() << shiftBits);
        }
      }
    private:
      int const mWidth;
      int const mHeight;
      std::unique_ptr<uint32_t[]> mPixels;
    };
  }
}