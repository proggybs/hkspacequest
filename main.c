#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include "settings.h"
#include "entity.h"
#include "system.h"

void glInit()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0, -1.0, 1.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 0.0);
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

void processKeypresses(Uint8 *keys, World *world, unsigned int entity)
{
  world->input[entity].keyLeft = keys[SDLK_LEFT];
  world->input[entity].keyRight = keys[SDLK_RIGHT];
  world->input[entity].keyFire = keys[SDLK_SPACE];
}

int getFontTexture(char* text)
{
  SDL_Color textColor = { 55, 255, 5 };
  TTF_Font *font = TTF_OpenFont("FreeSans.ttf", 92);
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, textColor);
  int colors = surface->format->BytesPerPixel;
  int format;
  GLuint texture;
  
  if(colors == 4)
  {
    if(surface->format->Rmask == 0x000000ff)
      format = GL_RGBA;
    else
      format = GL_BGRA;
  }
  else
  {
    if(surface->format->Rmask == 0x000000ff)
      format = GL_RGB;
    else
      format = GL_BGR;
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, colors, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
  return texture;
}

void drawFont(char* text)
{
  int texture = getFontTexture(text);
  glBindTexture(GL_TEXTURE_2D, texture);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0);
  glVertex2f(50, 500);
  glTexCoord2i(1, 0);
  glVertex2f(150, 500);
  glTexCoord2i(1, 1);
  glVertex2f(150, 530);
  glTexCoord2i(0, 1);
  glVertex2f(50, 530);
  glEnd();
  glDisable(GL_TEXTURE_2D );
  glPopMatrix();
  glFlush();
}


int main(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE|SDL_OPENGL);
  glInit();

  int isRunning = 1;
  Uint8 *keys = SDL_GetKeyState(NULL);

  World world;
  System render, player, movement, maxDuration, collision;

  memset(&world, 0, sizeof(world));
  initializeWorld(&world);

  unsigned int hkship = createHKShip(&world, (WINDOW_WIDTH/2 - 25), (WINDOW_HEIGHT - 60), 50.0f, 40.0f, 0.0f, 0.0f, "hkship1.png");
  createDrone(&world, (WINDOW_WIDTH/2 - 25), 60, 50.0f, 40.0f, 0.0f, 0.0f);

  unsigned int renderComps[2] = {COMPONENT_POSITION, COMPONENT_SPRITE};
  render.maskCount = 2;
  render.function = &renderFunction;
  memcpy(&render.mask, &renderComps, sizeof(renderComps)); 

  unsigned int playerComps[4] = {COMPONENT_INPUT, COMPONENT_POSITION, COMPONENT_VELOCITY, COMPONENT_FIRE_DELAY};
  player.maskCount = 4;
  player.function = &playerControlFunction;
  memcpy(&player.mask, &playerComps, sizeof(playerComps));

  unsigned int movementComps[2] = {COMPONENT_POSITION, COMPONENT_VELOCITY};
  movement.maskCount = 2;
  movement.function = &movementFunction;
  memcpy(&movement.mask, &movementComps, sizeof(movementComps));

  unsigned int maxDurationComps[1] = {COMPONENT_MAX_DURATION};
  maxDuration.maskCount = 1;
  maxDuration.function = &maxDurationFunction;
  memcpy(&maxDuration.mask, &maxDurationComps, sizeof(maxDurationComps));

  unsigned int collisionComps[2] = {COMPONENT_COLLISION, COMPONENT_POSITION};
  collision.maskCount = 2;
  collision.function = &collisionFunction;
  memcpy(&collision.mask, &collisionComps, sizeof(collisionComps));
  
  Uint32 start;
  Uint32 last = 1;
  while(isRunning)
  {
    start = SDL_GetTicks();

    runGameEvents(&isRunning);
    processKeypresses(keys, &world, hkship);

    runSystem(&player, &world);
    runSystem(&movement, &world);
    
    runSystem(&collision, &world);
    glClear(GL_COLOR_BUFFER_BIT);
    runSystem(&render, &world);
    char temp[8];
    sprintf(temp, "FPS: %d", (1000/last) ); 
    drawFont(temp);
    runSystem(&maxDuration, &world);
    SDL_GL_SwapBuffers();

    if(1000/60 > (SDL_GetTicks() - start))
      SDL_Delay(1000/60 - (SDL_GetTicks() - start));
    last = (SDL_GetTicks() - start);
  }

  TTF_Quit();
  SDL_Quit();
  return 0;
}
