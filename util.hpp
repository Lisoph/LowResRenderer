#pragma once

namespace LRR
{
  namespace Util
  {
    template <typename T>
    T Clamp(T && val, T && min, T && max) {
      return (val < min ? min : (val > max ? max : val));
    }

    template <typename T>
    T Lerp(T const &min, T const &max, float t) {
      return (1 - t) * min + t * max;
    }
  }
}