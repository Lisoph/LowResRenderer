#pragma once

namespace LRR
{
  namespace Util
  {
    template <typename T>
    T Clamp(T && val, T && min, T && max) {
      return (val < min ? min : (val > max ? max : val));
    }
  }
}