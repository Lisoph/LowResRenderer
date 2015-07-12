#include "Window.hpp"

namespace LRR
{
  inline void AssertOrThrow(bool expr, std::string const &why) {
    if(!expr) {
      throw WindowCreationException{why};
    }
  }

  Window::Window(int windowWidth, int windowHeight, int surfaceWidth,
                 int surfaceHeight, std::string const &title) {
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               windowWidth, windowHeight, 0);
    AssertOrThrow(mWindow != nullptr, "SDL_CreateWindow failed");

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    AssertOrThrow(mRenderer != nullptr, "SDL_CreateRenderer failed");

    mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING,
                                 surfaceWidth, surfaceHeight);
    AssertOrThrow(mTexture != nullptr, "SDL_CreateTexture failed");

    mSurface = SDL_CreateRGBSurface(0, surfaceWidth, surfaceHeight, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    AssertOrThrow(mSurface != nullptr, "SDL_CreateRGBSurface failed");
  }

  Window::Window(Window && other)
    : mWindow(other.mWindow),
    mRenderer(other.mRenderer),
    mTexture(other.mTexture),
    mSurface(other.mSurface) {
    other.mWindow = nullptr;
    other.mRenderer = nullptr;
    other.mTexture = nullptr;
    other.mSurface = nullptr;
  }

  Window::~Window() {
    if(mSurface) SDL_FreeSurface(mSurface);
    if(mTexture) SDL_DestroyTexture(mTexture);
    if(mRenderer) SDL_DestroyRenderer(mRenderer);
    if(mWindow) SDL_DestroyWindow(mWindow);
  }

  void Window::Present() {
    SDL_UpdateTexture(mTexture, nullptr, mSurface->pixels, mSurface->pitch);
    SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);
    SDL_RenderPresent(mRenderer);
  }
}