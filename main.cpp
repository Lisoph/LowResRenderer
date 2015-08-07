#include <cstdio>

// Visual Studio 2015 workaround
extern "C" FILE *__iob_func() {
  static FILE files[] = {*(stdin), *(stdout), *(stderr)};
  return files;
}

#include <SDL.h>

#include "Application.hpp"

int main(int, char**)
{
  SDL_Init(SDL_INIT_VIDEO);

  try {
    LRR::Application app;
    app.Run();
  }
  catch(std::exception &ex) {
    SDL_ShowSimpleMessageBox(0, "OH SNAP", ex.what(), nullptr);
  }

  SDL_Quit();
  return 0;
}