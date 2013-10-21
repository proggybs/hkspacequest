#ifndef HK_COMPONENT_H
#define HK_COMPONENT_H

#include <GL/gl.h>

#define COMPONENT_COUNT 4
#define COMPONENT_ENABLED 1
#define COMPONENT_DISABLED 0

const unsigned int COMPONENT_NONE;
const unsigned int COMPONENT_POSITION;
const unsigned int COMPONENT_VELOCITY;
const unsigned int COMPONENT_SPRITE;

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

unsigned int getMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask);
void setMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask, unsigned int enabled);
void resetMask(unsigned int (*em)[COMPONENT_COUNT]);
unsigned int checkMask(unsigned int (*sm)[], unsigned int msize, unsigned int (*em)[COMPONENT_COUNT]);

#endif
