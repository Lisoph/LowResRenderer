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

    mRasterizer.Clear(0);
    
    static Eigen::Vector2i a{50, 0};
    static Eigen::Vector2i b{0, 50};
    static Eigen::Vector2i c{70, 100};

    /*if(frame++ % 300 == 0) {
      a(0) = rand() % bitmap.Width();
      a(1) = rand() % bitmap.Height();
      b(0) = rand() % bitmap.Width();
      b(1) = rand() % bitmap.Height();
      c(0) = rand() % bitmap.Width();
      c(1) = rand() % bitmap.Height();
    }*/

    //float timeA = SDL_GetTicks() / 1000.0f - 22.5f;
    float timeA = mMouseWheelPos * 0.1f;
    float timeB = timeA + 22.5f;
    float timeC = timeB + 22.5f;
    a(0) = width / 2 + std::cos(timeA * 22.5f * 0.01745329251f) * (width / 2);
    a(1) = height / 2 + std::sin(timeA * 22.5f * 0.01745329251f) * (height / 2);
    b(0) = width / 2 + std::cos(timeB * 22.5f * 0.01745329251f) * (width / 2);
    b(1) = height / 2 + std::sin(timeB * 22.5f * 0.01745329251f) * (height / 2);
    c(0) = width / 2 + std::cos(timeC * 22.5f * 0.01745329251f) * (width / 2);
    c(1) = height / 2 + std::sin(timeC * 22.5f * 0.01745329251f) * (height / 2);

    mRasterizer.DrawScanlineBuffer(0, 0, Rendering::ScanlineBuffer::Triangle(a, b, c));

    for(int i = 0; i < max; ++i) {
      mWindow.Pixels()[i] = bitmap.GetPixel(i);
    }

    mWindow.Present();

    auto *renderer = mWindow.Renderer();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, float(a(0)) / width * windowWidth, float(a(1)) / height * windowHeight,
                       float(b(0)) / width * windowWidth, float(b(1)) / height * windowHeight);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, float(b(0)) / width * windowWidth, float(b(1)) / height * windowHeight,
                       float(c(0)) / width * windowWidth, float(c(1)) / height * windowHeight);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, float(c(0)) / width * windowWidth, float(c(1)) / height * windowHeight,
                       float(a(0)) / width * windowWidth, float(a(1)) / height * windowHeight);
    SDL_RenderPresent(renderer);
  }
}