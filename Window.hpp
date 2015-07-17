#pragma once

#include <string>
#include <SDL.h>

namespace LRR
{
  class WindowCreationException: std::exception
  {
  public:
    WindowCreationException(std::string const &why)
    {}

    const char *what() const override {
      return mWhy.c_str();
    }

  private:
    std::string mWhy;
  };

  class Window final
  {
  public:
    Window() = delete;
    Window(int windowWidth, int windowHeight,
           int surfaceWidth, int surfaceHeight, std::string const &title);
    Window(Window const&) = delete;
    Window(Window && other);
    ~Window();

    inline SDL_Renderer *Renderer() const {
      return mRenderer;
    }

    inline void WindowSize(int &outWidth, int &outHeight) const {
      SDL_GetWindowSize(mWindow, &outWidth, &outHeight);
    }

    inline void SurfaceSize(int &outWidth, int &outHeight) const {
      outWidth = mSurface->w;
      outHeight = mSurface->h;
    }

    inline Uint32 *Pixels()
    { return (Uint32*)mSurface->pixels; }

    void Present();
  private:
    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    SDL_Texture *mTexture = nullptr;
    SDL_Surface *mSurface = nullptr;
  };
}