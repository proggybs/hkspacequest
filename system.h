#ifndef HK_SYSTEM_H
#define HK_SYSTEM_H

#include "component.h"
#include "entity.h"

typedef void (* SystemFunction)(World *world, unsigned int entity);

typedef struct
{
  SystemFunction function;
  int maskCount;
  unsigned int mask[COMPONENT_COUNT];
} System;

void runSystem(System *system, World *world);
void renderFunction(World *world, unsigned int entity);
void playerControlFunction(World *world, unsigned int entity);
void movementFunction(World *world, unsigned int entity);
void maxDurationFunction(World *world, unsigned int entity);
void collisionFunction(World *world, unsigned int entity);
#endif
