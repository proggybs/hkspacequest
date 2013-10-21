#include <SDL/SDL.h>
#include <GL/gl.h>
#include "entity.h"
#include "system.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void glInit()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0, -1.0, 1.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void runGameEvents(int *isRunning)
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT:
        *isRunning = 0;
        break;
      default:
        break;
    }
  }
}

int main(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE|SDL_OPENGL);
  glInit();

  int isRunning = 1;
  Uint32 start;

  World world;
 System render;

  unsigned int hkship;

  memset(&world, 0, sizeof(world));
  initializeWorld(&world);
  hkship = createHKShip(&world, (WINDOW_WIDTH/2 - 25), (WINDOW_HEIGHT - 60), 50.0f, 40.0f, 0.0f, 0.0f, "hkship1.png");
  unsigned int comps[2] = {COMPONENT_POSITION, COMPONENT_SPRITE};
  render.maskCount = 2;
  render.function = &renderFunction;
  memcpy(&render.mask, &comps, sizeof(comps)); 
  while(isRunning)
  {
    start = SDL_GetTicks();

    runGameEvents(&isRunning);
    glClear(GL_COLOR_BUFFER_BIT);

    runSystem(&render, &world);

    SDL_GL_SwapBuffers();
    if(1000/60 > (SDL_GetTicks() - start))
      SDL_Delay(1000/60 - (SDL_GetTicks() - start));
  }

  SDL_Quit();
  return 0;
}
