#ifndef HK_COMPONENT_H
#define HK_COMPONENT_H

#include <GL/gl.h>

#define COMPONENT_COUNT 9
#define COMPONENT_ENABLED 1 
#define COMPONENT_DISABLED 0 

const unsigned int COMPONENT_NONE;
const unsigned int COMPONENT_POSITION;
const unsigned int COMPONENT_VELOCITY;
const unsigned int COMPONENT_SPRITE;
const unsigned int COMPONENT_INPUT;
const unsigned int COMPONENT_MAX_DURATION;
const unsigned int COMPONENT_COLLISION;
const unsigned int COMPONENT_AI;
const unsigned int COMPONENT_FIRE_DELAY;

typedef struct
{
  float x;
  float y;
  float h;
  float w;
} Position;

typedef struct
{
  float x;
  float y;
} Velocity;

typedef struct
{
  GLuint texture;
} Sprite;

typedef struct
{
  int keyLeft;
  int keyRight;
  int keyFire;
} Input;

typedef struct
{
  int current;
  int max;
} MaxDuration;

typedef struct
{
  unsigned int collidedWith;
} Collision;

typedef struct
{
  int lastMove;
  int moveDuration;
  int moveCount;
  int moveSpeed;
  int moveMax;
} AI;

typedef struct
{
  unsigned int timeSinceFired;
  unsigned int delay;
} FireDelay;

#endif
