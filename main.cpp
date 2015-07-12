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