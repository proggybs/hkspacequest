#include "entity.h"
#include "stdio.h"
#include "texture.h"
#include "settings.h"

unsigned int createEntity(World *world)
{
  unsigned int entity;

  for(entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    if(getMask(&world->mask[entity], COMPONENT_NONE))
    {
      setMask(&world->mask[entity], COMPONENT_NONE, COMPONENT_DISABLED);
      printf("created entity %i\n", entity);
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

  return entity;
}

unsigned int createBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int maxDuration)
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

  world->maxDuration[entity].max = maxDuration;
  world->maxDuration[entity].current = 0;

  return entity;
}
