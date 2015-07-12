#include "Bitmap.hpp"

#include <cstring>

namespace LRR
{
  namespace Rendering
  {
    Bitmap::Bitmap(int width, int height)
      : mWidth(width),
      mHeight(height),
      mPixels(new uint32_t[width * height])
    {}

    Bitmap::Bitmap(Bitmap const &other)
      : Bitmap(other.mWidth, other.mHeight) {
      memcpy(mPixels.get(), other.mPixels.get(), sizeof(uint32_t) * mWidth * mHeight);
    }

    Bitmap::Bitmap(Bitmap && other)
      : mWidth(other.mWidth),
      mHeight(other.mHeight),
      mPixels(std::move(other.mPixels))
    {}
  }
}