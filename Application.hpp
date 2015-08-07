#pragma once

#include "Window.hpp"
#include "Rasterizer.hpp"
#include "BasicShader.hpp"

namespace LRR
{
  class Application final
  {
  public:
    static const float UpdateTimeStep;

    static constexpr unsigned PixelScale = 4;
    static constexpr unsigned WindowWidth = 640;
    static constexpr unsigned WindowHeight = 480;
    static constexpr unsigned SurfaceWidth = WindowWidth / PixelScale;
    static constexpr unsigned SurfaceHeight = WindowHeight / PixelScale;

    void Run();
  private:
    void HandleEvents();
    void Update();
    void Draw();

    inline void Quit()
    { mRunning = false; }

    bool mRunning = true;
    float mSimulationTime = 0;
    float mScroll = 0;
    
    Window mWindow{WindowWidth, WindowHeight, SurfaceWidth, SurfaceHeight, "Low Res Software Renderer"};
    Rendering::Rasterizer mRasterizer{SurfaceWidth, SurfaceHeight};
    Rendering::BasicShader mShader{float(SurfaceWidth) / (float)SurfaceHeight};
  };
}