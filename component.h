#ifndef HK_COMPONENT_H
#define HK_COMPONENT_H

#define COMPONENT_COUNT 3
#define COMPONENT_ENABLED 1
#define COMPONENT_DISABLED 0

const unsigned int COMPONENT_NONE;
const unsigned int COMPONENT_DISPLACEMENT;
const unsigned int COMPONENT_VELOCITY;
unsigned int SystemMask[COMPONENT_COUNT];

typedef struct
{
  float x;
  float y;
} Position;

typedef struct
{
  float x;
  float y;
} Velocity;

unsigned int getMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask);
void setMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask, unsigned int enabled);
void resetMask(unsigned int (*em)[COMPONENT_COUNT]);
unsigned int checkMask(unsigned int (*sm)[], unsigned int msize, unsigned int (*em)[COMPONENT_COUNT]);

#endif
