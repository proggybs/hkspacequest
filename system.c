#include "settings.h"
#include "system.h"
#include "component.h"
#include "entity.h"
#include <GL/gl.h>
#include <stdio.h>

void runSystem(System *system, World *world)
{
  unsigned int entity;
  for(entity = 0; entity < ENTITY_COUNT; ++entity)
  {
    int check = 0;
    for(unsigned int x = 0; x < system->maskCount; ++x)
    {
      check = 1;
      if((world->mask[entity][x] & system->mask[x]) == COMPONENT_ENABLED)
        continue;
      else
        break;
    }

    if(check)
      system->function(world, entity);
  }
}

void renderFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);
  
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
  glPopMatrix();
  glFlush();
}

void playerControlFunction(World *world, unsigned int entity)
{
  Input *i = &(world->input[entity]);
  Position *p = &(world->position[entity]);
  Velocity *v = &(world->velocity[entity]);

  v->x = 0.0f;

  if(i->keyLeft && (p->x > 10))
      v->x = -10.0f;
  
  if(i->keyRight && (p->x < (WINDOW_WIDTH - 60)))
      v->x = 10.0f;
  
  if(!i->keyRight && !i->keyLeft)
    v->x = 0.0f;
  
  if(i->keyFire)
    createBlaster(world, p->x, p->y - 20, 10.0f, 20.0f, 0.0f, -10.0f, 4); 
}

void movementFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);
  Velocity *v = &(world->velocity[entity]);

  p->x += v->x;
  p->y += v->y;
}


void maxDurationFunction(World *world, unsigned int entity)
{
  puts("called\n");
  MaxDuration *md = &(world->maxDuration[entity]);

  if(md->current >= md->max)
    destroyEntity(world, entity);
}
