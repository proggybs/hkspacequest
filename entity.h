#ifndef HK_ENTITY_H
#define HK_ENTITY_H

#include "component.h"

#define ENTITY_COUNT 100

typedef struct
{
  unsigned int mask[ENTITY_COUNT][COMPONENT_COUNT];

  Position position[ENTITY_COUNT];
  Velocity velocity[ENTITY_COUNT];
} World;

unsigned int createEntity(World *world);
void destroyEntity(World *world, unsigned int entity);
void initializeWorld(World *world);

#endif
