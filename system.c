#include "system.h"
#include "component.h"
#include "entity.h"
#include <GL/gl.h>
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
  Sprite *s = &(world->sprite[entity]);

  glBindTexture(GL_TEXTURE_2D, s->texture);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0);
  glVertex3f(p->x, p->y, 0);
  glTexCoord2i(1, 0);
  glVertex3f(p->x + p->w, p->y, 0);
  glTexCoord2i(1, 1);
  glVertex3f(p->x + p->w, p->y + p->h, 0);
  glTexCoord2i(0, 1);
  glVertex3f(p->x, p->y + p->h, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D );
  glPopMatrix();
  glFlush();
}
