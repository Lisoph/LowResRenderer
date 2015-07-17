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

    enum { PixelScale = 4 };
    enum { WindowWidth = 640 };
    enum { WindowHeight = 480 };
    enum { SurfaceWidth = WindowWidth / PixelScale };
    enum { SurfaceHeight = WindowHeight / PixelScale };

    Application() {}

    void Run();
  private:
    void HandleEvents();
    void Update();
    void Draw();

    inline void Quit()
    { mRunning = false; }

    bool mRunning = true;
    float mSimulationTime = 0;
    
    Window mWindow{WindowWidth, WindowHeight, SurfaceWidth, SurfaceHeight, "Low Res Software Renderer"};
    Rendering::Rasterizer mRasterizer{SurfaceWidth, SurfaceHeight};
    Rendering::BasicShader mShader{float(SurfaceWidth) / (float)SurfaceHeight};
  };
}