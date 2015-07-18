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
      else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) {
          Quit();
        }
      }
    }
  }

  void Application::Update() {
    float distance = SDL_GetTicks() / 1000.0f;
    float angle = Math::ToRadians(distance * 90);
    mShader.ModelMatrix(Math::RotationMatrix(Eigen::AngleAxisf{angle, Eigen::Vector3f{0.0f, 1.0f, 0.0f}}));

    auto view = mShader.ViewMatrix();
    view(14) = -7;
    mShader.ViewMatrix(view);
  }

  void Application::Draw() {
    mRasterizer.Clear(0x89CFF0);
    mRasterizer.DrawTriangle({0, 1, 0}, {-1, -1, 0}, {1, -1, 0}, mShader);

    auto &bitmap = mRasterizer.Bitmap();
    int width, height;
    mWindow.SurfaceSize(width, height);
    int const max = width * height;

    for(int i = 0; i < max; ++i) {
      mWindow.Pixels()[i] = bitmap.GetPixel(i);
    }

    mWindow.Present();
  }
}