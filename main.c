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
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
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

typedef struct
{
  unsigned int entity;
  int destinations[5];
  int cd;
} Drone;

Drone drones[10];

void createDrones(World *world)
{
  for(int x = 0; x < 10; ++x)
  {
    drones[x].entity = createDumbDrone(world, -20 - (x*25), 500, 20.0f, 20.0f);
    drones[x].destinations[0] = 200;
    drones[x].destinations[1] = 100;
    drones[x].destinations[2] = 200 + (x * 50);
    drones[x].cd = 0;
    world->velocity[drones[x].entity].x = 5;
  }
}
void moveDrones(World *world)
{
  for(int x = 0; x < 10; ++x)
  {
    if(drones[x].cd == 0 && world->position[drones[x].entity].x == drones[x].destinations[0])
    {
      world->velocity[drones[x].entity].x = 0;
      world->velocity[drones[x].entity].y = -5;
      drones[x].cd = 1;
    }
    if(drones[x].cd == 1 && world->position[drones[x].entity].y == drones[x].destinations[1])
    {
      world->velocity[drones[x].entity].y = 0;
      world->velocity[drones[x].entity].x = 5;
      drones[x].cd = 2;
    }
    if(drones[x].cd == 2 && world->position[drones[x].entity].x == drones[x].destinations[2])
    {
      world->velocity[drones[x].entity].x = 0;
      drones[x].cd = 3;
    }
  }

  if(drones[9].cd == 3)
  {
    for(int x = 0; x < 10; ++x)
    {
      world->mask[drones[x].entity][COMPONENT_AI] = COMPONENT_ENABLED;
      drones[x].cd = 4;
    }
  }
}

int main(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL);
  glInit();

  int isRunning = 1;
  Uint8 *keys = SDL_GetKeyState(NULL);

  World world;
  System render, player, movement, maxDuration, collision, collisionR, pfCollision, aifCollision, aiMove, aiFire;

  memset(&world, 0, sizeof(world));
  initializeWorld(&world);

  unsigned int hkship = createHKShip(&world, (WINDOW_WIDTH/2 - 25), (WINDOW_HEIGHT - 60), 50.0f, 40.0f, 0.0f, 0.0f, "hkship1.png");

  createDrones(&world);

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

  unsigned int collisionComps[2] = {COMPONENT_COLLIDABLE, COMPONENT_POSITION};
  collision.maskCount = 2;
  collision.function = &collisionDetectionFunction;
  memcpy(&collision.mask, &collisionComps, sizeof(collisionComps));

  unsigned int collisionRComps[1] = {COMPONENT_COLLIDABLE};
  collisionR.maskCount = 1;
  collisionR.function = &collisionActionFunction;
  memcpy(&collisionR.mask, &collisionRComps, sizeof(collisionRComps));

  unsigned int aiMoveComps[3] = {COMPONENT_POSITION, COMPONENT_VELOCITY, COMPONENT_AI};
  aiMove.maskCount = 3;
  aiMove.function = &moveAIFunction;
  memcpy(&aiMove.mask, &aiMoveComps, sizeof(aiMoveComps));

  unsigned int aiFireComps[3] = {COMPONENT_POSITION, COMPONENT_FIRE_DELAY, COMPONENT_AI};
  aiFire.maskCount = 3;
  aiFire.function = &fireAIFunction;
  memcpy(&aiFire.mask, &aiFireComps, sizeof(aiFireComps));

  unsigned int pfcComps[2] = {COMPONENT_PLAYER_FRIENDLY, COMPONENT_COLLIDABLE};
  pfCollision.maskCount = 2;
  pfCollision.function = &playerFriendlyCollisionFunction;
  memcpy(&pfCollision.mask, &pfcComps, sizeof(pfcComps));

  unsigned int aicComps[2] = {COMPONENT_AI_FRIENDLY, COMPONENT_COLLIDABLE};
  aifCollision.maskCount = 2;
  aifCollision.function = &aiFriendlyCollisionFunction;
  memcpy(&aifCollision.mask, &aicComps, sizeof(aicComps));

  Uint32 start;
  Uint32 last = 1;
  while(isRunning)
  {
    start = SDL_GetTicks();

    runGameEvents(&isRunning);
    processKeypresses(keys, &world, hkship);
    runSystem(&player, &world);
    runSystem(&aiMove, &world);
    runSystem(&movement, &world);
    runSystem(&aiFire, &world);

    runSystem(&collision, &world);
    runSystem(&pfCollision, &world);
    runSystem(&aifCollision, &world);
    runSystem(&collisionR, &world);
    glClear(GL_COLOR_BUFFER_BIT);
    runSystem(&render, &world);
    moveDrones(&world);
    char temp[8];
    sprintf(temp, "FPS: %d", (1000/last) ); 
    drawFont(temp);
    runSystem(&maxDuration, &world);
    SDL_GL_SwapBuffers();

    if(1000/60 > (SDL_GetTicks() - start))
      SDL_Delay(1000/60 - (SDL_GetTicks() - start));
    last = (SDL_GetTicks() - start);
  }

  SDL_FreeSurface(screen);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
