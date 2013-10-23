#include "entity.h"
#include "stdio.h"
#include "texture.h"
#include "settings.h"
#include <SDL/SDL.h>

unsigned int createEntity(World *world)
{
  unsigned int entity;

  for(entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    if(world->mask[entity][COMPONENT_NONE] & COMPONENT_ENABLED)
    {
      world->mask[entity][COMPONENT_NONE] = COMPONENT_DISABLED;
      return entity;
    }
  }

  printf("Error! No more entities left! Max is %d\n", ENTITY_COUNT);
  return ENTITY_COUNT;
}

void destroyEntity(World *world, unsigned int entity)
{
  for(unsigned int mask = 1; mask < COMPONENT_COUNT; ++mask)
  {
    world->mask[entity][mask] = COMPONENT_DISABLED;
  }
  world->mask[entity][COMPONENT_NONE] = COMPONENT_ENABLED;
}

void initializeWorld(World *world)
{
  for(unsigned int entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    for(unsigned int mask = 1; mask < COMPONENT_COUNT; ++mask)
    {
      world->mask[entity][mask] = COMPONENT_DISABLED;
    }
    world->mask[entity][COMPONENT_NONE] = COMPONENT_ENABLED;
  }
}

unsigned int createHKShip(World *world, float x, float y, float w, float h, float vx, float vy, char* filename)
{
  unsigned int entity = createEntity(world);

  world->mask[entity][COMPONENT_POSITION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_VELOCITY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_SPRITE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_INPUT] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_COLLIDABLE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_FIRE_DELAY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_PLAYER_FRIENDLY] = COMPONENT_ENABLED;

  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;

  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->input[entity].keyLeft = 0;
  world->input[entity].keyRight = 0;
  world->input[entity].keyFire = 0;

  world->sprite[entity].texture = 1;

  world->fireDelay[entity].timeSinceFired = 0;
  world->fireDelay[entity].delay = 500;

  return entity;
}

unsigned int createDrone(World *world, float x, float y, float w, float h, float vx, float vy)
{
  unsigned int entity = createEntity(world);

  world->mask[entity][COMPONENT_POSITION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_VELOCITY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_SPRITE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_COLLIDABLE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_AI] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_FIRE_DELAY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_AI_FRIENDLY] = COMPONENT_ENABLED;
  
  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;

  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->ai[entity].lastMove = SDL_GetTicks();
  world->ai[entity].moveDuration = 0;
  world->ai[entity].moveCount = 0;
  world->ai[entity].moveSpeed = 5;
  world->ai[entity].moveMax = 20;
  
  world->fireDelay[entity].timeSinceFired = 0;
  world->fireDelay[entity].delay = 0;

  return entity;
}

unsigned int createFriendlyBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int maxDuration)
{
  unsigned int entity = createEntity(world);

  world->mask[entity][COMPONENT_POSITION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_VELOCITY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_SPRITE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_MAX_DURATION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_COLLIDABLE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_PLAYER_FRIENDLY] = COMPONENT_ENABLED;

  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;

  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->maxDuration[entity].max = maxDuration;
  world->maxDuration[entity].current = 0;

  return entity;
}

unsigned int createEnemyBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int maxDuration)
{
  unsigned int entity = createEntity(world);

  world->mask[entity][COMPONENT_POSITION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_VELOCITY] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_SPRITE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_MAX_DURATION] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_COLLIDABLE] = COMPONENT_ENABLED;
  world->mask[entity][COMPONENT_AI_FRIENDLY] = COMPONENT_ENABLED;

  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;

  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->maxDuration[entity].max = maxDuration;
  world->maxDuration[entity].current = 0;

  return entity;
}
