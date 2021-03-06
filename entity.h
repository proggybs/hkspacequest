#ifndef HK_ENTITY_H
#define HK_ENTITY_H

#include "component.h"


typedef struct
{
  unsigned int mask[ENTITY_COUNT][COMPONENT_COUNT];

  Position position[ENTITY_COUNT];
  Velocity velocity[ENTITY_COUNT];
  Sprite sprite[ENTITY_COUNT];
  Input input[ENTITY_COUNT];
  MaxDuration maxDuration[ENTITY_COUNT];
  Collidable collidable[ENTITY_COUNT];
  AI ai[ENTITY_COUNT];
  FireDelay fireDelay[ENTITY_COUNT];
} World;

unsigned int createEntity(World *world);
void destroyEntity(World *world, unsigned int entity);
void initializeWorld(World *world);

unsigned int createHKShip(World *world, float x, float y, float w, float h, float vx, float vy, char* filename);
unsigned int createFriendlyBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int frames);
unsigned int createEnemyBlaster(World *world, float x, float y, float w, float h, float vx, float vy, int frames);
unsigned int createDrone(World *world, float x, float y, float w, float h, float vx, float vy);
unsigned int createDumbDrone(World *world, float x, float y, float w, float h);
#endif
