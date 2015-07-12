#pragma once

#include "Window.hpp"
#include "Rasterizer.hpp"

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

    //static constexpr float UpdateTimeStep = 16.0f;

    /*static constexpr int PixelScale = 4;
    static constexpr int WindowWidth = 640;
    static constexpr int WindowHeight = 480;
    static constexpr int SurfaceWidth = WindowWidth / PixelScale;
    static constexpr int SurfaceHeight = WindowHeight / PixelScale;*/

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
    float mMouseWheelPos = 0;
    
    Window mWindow{WindowWidth, WindowHeight, SurfaceWidth, SurfaceHeight, "Low Res Software Renderer"};
    Rendering::Rasterizer mRasterizer{SurfaceWidth, SurfaceHeight};
  };
}