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
  /*Sprite *s = &(world->sprite[entity]);*/

  /*
    glBindTexture(GL_TEXTURE_2D, s->texture);
    glEnable(GL_TEXTURE_2D);
    glTexCoord2i(0, 0);
  */
  glPushMatrix();
  glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.5);
  glVertex2f(p->x, p->y);
    glColor3f(1.0, 0.5, 1.0);
  glVertex2f(p->x + p->w, p->y);
    glColor3f(0.5, 1.0, 1.0);
  glVertex2f(p->x + p->w, p->y + p->h);
    glColor3f(0.5, 0.5, 0.5);
  glVertex2f(p->x, p->y + p->h);
  glEnd();
  /*glDisable(GL_TEXTURE_2D );*/
  glPopMatrix();
  glFlush();
}

void playerControlFunction(World *world, unsigned int entity)
{
  Input *i = &(world->input[entity]);
  Velocity *v = &(world->velocity[entity]);

  if(i->keyLeft)
    v->x = -10.0f;
  
  if(i->keyRight)
    v->x = 10.0f;
  
  if((!i->keyRight && !i->keyLeft) || (i->keyRight && i->keyLeft))
    v->x = 0.0f;
}

void movementFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);
  Velocity *v = &(world->velocity[entity]);

  p->x += v->x;
  p->y += v->y;
}
