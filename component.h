#ifndef HK_COMPONENT_H
#define HK_COMPONENT_H

#include <GL/gl.h>

#define COMPONENT_COUNT 6
const unsigned int COMPONENT_ENABLED; 
const unsigned int COMPONENT_DISABLED; 

const unsigned int COMPONENT_NONE;
const unsigned int COMPONENT_POSITION;
const unsigned int COMPONENT_VELOCITY;
const unsigned int COMPONENT_SPRITE;
const unsigned int COMPONENT_INPUT;
const unsigned int COMPONENT_MAX_DURATION;

unsigned int SystemMask[COMPONENT_COUNT];

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

unsigned int getMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask);
void setMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask, unsigned int enabled);
void resetMask(unsigned int (*em)[COMPONENT_COUNT]);
unsigned int checkMask(unsigned int (*sm)[], unsigned int msize, unsigned int (*em)[COMPONENT_COUNT]);

#endif
