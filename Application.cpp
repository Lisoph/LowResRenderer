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
    
    auto mat = mShader.ViewMatrix();
    mat(14) = -5;
    mShader.ViewMatrix(mat);

    /*Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
    mat(13) = distance;
    mShader.ModelMatrix(mat);*/
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

    if(frame++ % 150 == 0) {
#if 0
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
#endif

    }

    //mRasterizer.DrawScanlineBuffer(0, 0, Rendering::ScanlineBuffer::Triangle(a, b, c));
    //mRasterizer.DrawScanlineBuffer(0, 0, Rendering::ScanlineBuffer::Triangle(a2, b2, c2));

    mRasterizer.DrawTriangle({0, 1, 0}, {-1, -1, 0}, {1, -1, 0}, mShader);

    for(int i = 0; i < max; ++i) {
      mWindow.Pixels()[i] = bitmap.GetPixel(i);
    }

    mWindow.Present();
  }
}