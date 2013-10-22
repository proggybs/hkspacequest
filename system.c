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
    if(world->mask[entity][COMPONENT_NONE] == COMPONENT_DISABLED)
    {
      int check = 1;
      for(int x = 0; x < system->maskCount; ++x)
      {
        if(!(world->mask[entity][system->mask[x]] & COMPONENT_ENABLED))
        {
          check = 0;
          break;
        }
      }

      if(check)
        system->function(world, entity);
    }
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
    createBlaster(world, p->x, p->y - 20, 10.0f, 20.0f, 0.0f, -10.0f, 60); 
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
  MaxDuration *md = &(world->maxDuration[entity]);

  md->current++;
  if(md->current >= md->max)
    destroyEntity(world, entity);
}

void collisionFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);

  unsigned int e2;
  for(e2 = 0; e2 < ENTITY_COUNT; ++e2)
  {
    if(entity != e2 && (world->mask[e2][COMPONENT_COLLISION] & COMPONENT_ENABLED))
    {
      Position *p2 = &(world->position[e2]);

      float bottom1 = p->y + p->h;
      float bottom2 = p2->y + p2->h;
      float top1 = p->y;
      float top2 = p2->y;
      float left1 = p->x;
      float left2 = p2->x;
      float right1 = p->x + p->w;
      float right2 = p2->x + p2->w;

      if (bottom1 < top2) continue;
      if (top1 > bottom2) continue;
      if (right1 < left2) continue;
      if (left1 > right2) continue;

      destroyEntity(world, entity);
      destroyEntity(world, e2);
    }
  }
}
