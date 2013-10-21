#include "entity.h"
#include "stdio.h"
#include "texture.h"

unsigned int createEntity(World *world)
{
  unsigned int entity;
  
  for(entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    if(getMask(&world->mask[entity], COMPONENT_NONE))
    {
      setMask(&world->mask[entity], COMPONENT_NONE, COMPONENT_DISABLED);
      return entity;
    }
  }

  printf("Error! No more entities left! Max is %d", ENTITY_COUNT);
  return ENTITY_COUNT;
}

void destroyEntity(World *world, unsigned int entity)
{
  resetMask(&world->mask[entity]);
}

void initializeWorld(World *world)
{
  for(int x = 0; x < ENTITY_COUNT; ++x)
    resetMask(&world->mask[x]);
}

unsigned int createHKShip(World *world, float x, float y, float w, float h, float vx, float vy, char* filename)
{
  unsigned int entity = createEntity(world);

  setMask(&world->mask[entity], COMPONENT_POSITION, COMPONENT_ENABLED);
  setMask(&world->mask[entity], COMPONENT_VELOCITY, COMPONENT_ENABLED);
  setMask(&world->mask[entity], COMPONENT_SPRITE, COMPONENT_ENABLED);
  setMask(&world->mask[entity], COMPONENT_INPUT, COMPONENT_ENABLED);

  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;
  
  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->input[entity].keyLeft = 0;
  world->input[entity].keyRight = 0;
  world->input[entity].keyFire = 0;

  return entity;
}

unsigned int createBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int frames)
{
  unsigned int entity = createEntity(world);

  setMask(&world->mask[entity], COMPONENT_POSITION, COMPONENT_ENABLED);
  setMask(&world->mask[entity], COMPONENT_VELOCITY, COMPONENT_ENABLED);
  setMask(&world->mask[entity], COMPONENT_SPRITE, COMPONENT_ENABLED);
  
  world->position[entity].x = x;
  world->position[entity].y = y;
  world->position[entity].w = w;
  world->position[entity].h = h;
  
  world->velocity[entity].x = vx;
  world->velocity[entity].y = vy;

  world->frames[entity] = frames;

  return entity;
}
