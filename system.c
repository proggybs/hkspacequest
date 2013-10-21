#include "system.h"
#include "component.h"
#include "entity.h"
#include <stdio.h>

void runSystem(System *system, World *world)
{
  unsigned int entity;
  for(entity = 0; entity < ENTITY_COUNT; ++entity)
    if(checkMask(&system->mask, system->maskCount, &world->mask[entity]))
      system->function(world, entity);
}

void renderFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);
  /*Appearance *a = &(world->appearance[entity]);*/

  printf("rendered entity at (%f, %f)", p->x, p->y);
}
