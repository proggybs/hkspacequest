#include "component.h"

const unsigned int COMPONENT_NONE = 0;
const unsigned int COMPONENT_POSITION = 1;
const unsigned int COMPONENT_VELOCITY = 2;
const unsigned int COMPONENT_SPRITE = 3;

unsigned int checkMask(unsigned int (*sm)[], unsigned int msize, unsigned int (*em)[COMPONENT_COUNT])
{
  for(unsigned int x = 0; x < msize; ++x)
  {
    if((*em)[(*sm)[x]] & COMPONENT_DISABLED)
      return 0;
  }

  return 1;
}

unsigned int getMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask)
{
  return (*em)[mask] & COMPONENT_ENABLED;
}
 
void setMask(unsigned int (*em)[COMPONENT_COUNT], unsigned int mask, unsigned int enabled)
{
  (*em)[mask] = enabled;
}

void resetMask(unsigned int (*em)[COMPONENT_COUNT])
{
  for(int x = 1; x < COMPONENT_COUNT; ++x)
    (*em)[x] = COMPONENT_DISABLED;

  (*em)[COMPONENT_NONE] = COMPONENT_ENABLED;
}
