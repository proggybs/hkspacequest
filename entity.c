#include "entity.h"
#include "stdio.h"

unsigned int createEntity(World *world)
{
  unsigned int entity;
  
  for(entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    if(getMask(&world->mask[entity], COMPONENT_NONE))
    {
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
