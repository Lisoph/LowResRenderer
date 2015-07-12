#include "Application.hpp"

namespace LRR
{
  const float Application::UpdateTimeStep = 16.0f;

  void Application::Run() {
    while(mRunning) {
      HandleEvents();

      float realTime = (float)SDL_GetTicks();
      for(; mSimulationTime < realTime; mSimulationTime += UpdateTimeStep) {
        Update();
      }

      Draw();
    }
  }

  void Application::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        Quit();
      }
      else if(event.type == SDL_MOUSEWHEEL) {
        mMouseWheelPos += event.wheel.y;
      }
    }
  }

  void Application::Update() {

  }

  void Application::Draw() {
    auto &bitmap = mRasterizer.Bitmap();
    int width, height;
    mWindow.SurfaceSize(width, height);
    int const max = width * height;
    int windowWidth, windowHeight;
    mWindow.WindowSize(windowWidth, windowHeight);

    static int frame = 0;

    mRasterizer.Clear(0x89CFF0);
    
    static Eigen::Vector2i a{50, 0};
    static Eigen::Vector2i b{0, 50};
    static Eigen::Vector2i c{70, 100};

    static Eigen::Vector2i a2{50, 0};
    static Eigen::Vector2i b2{0, 50};
    static Eigen::Vector2i c2{70, 100};

    if(frame++ % 300 == 0) {
      a(0) = rand() % bitmap.Width();
      a(1) = rand() % bitmap.Height();
      b(0) = rand() % bitmap.Width();
      b(1) = rand() % bitmap.Height();
      c(0) = rand() % bitmap.Width();
      c(1) = rand() % bitmap.Height();

      a2(0) = rand() % bitmap.Width();
      a2(1) = rand() % bitmap.Height();
      b2(0) = rand() % bitmap.Width();
      b2(1) = rand() % bitmap.Height();
      c2(0) = rand() % bitmap.Width();
      c2(1) = rand() % bitmap.Height();
    }

    mRasterizer.DrawScanlineBuffer(0, 0, Rendering::ScanlineBuffer::Triangle(a, b, c));
    mRasterizer.DrawScanlineBuffer(0, 0, Rendering::ScanlineBuffer::Triangle(a2, b2, c2));

    for(int i = 0; i < max; ++i) {
      mWindow.Pixels()[i] = bitmap.GetPixel(i);
    }

    mWindow.Present();
  }
}