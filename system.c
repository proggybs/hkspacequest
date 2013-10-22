#include "settings.h"
#include "system.h"
#include "component.h"
#include "entity.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
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
  FireDelay *fd = &(world->fireDelay[entity]);

  v->x = 0.0f;

  if(i->keyLeft && (p->x > 10))
    v->x = -10.0f;

  if(i->keyRight && (p->x < (WINDOW_WIDTH - 60)))
    v->x = 10.0f;

  if(!i->keyRight && !i->keyLeft)
    v->x = 0.0f;

  if(i->keyFire && (fd->timeSinceFired < (SDL_GetTicks() - fd->delay)))
  {
    createBlaster(world, p->x, p->y - 20, 10.0f, 20.0f, 0.0f, -10.0f, 60); 
    fd->timeSinceFired = SDL_GetTicks();
  }
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

void moveAIFunction(World *world, unsigned int entity)
{
  Position *p = &(world->position[entity]);
  Velocity *v = &(world->velocity[entity]);
  AI *ai = &(world->ai[entity]);

  srand(SDL_GetTicks() + entity);
  int randRoll = rand() % 100 + 1;
  if(v->x == 0 && v->y == 0 && (randRoll == 1)) // Not Moving
  {
    int direction;

    if(p->x < 10)
      direction = 4;
    else if(p->x > WINDOW_WIDTH - (10 + p->w))
      direction = 2;
    else if(p->y < 10)
      direction = 1;
    else if(p->y > WINDOW_HEIGHT - 200)
      direction = 3;
    else
      direction = rand() % 4 + 1;

    if(direction == 1) // Down
      v->y += ai->moveSpeed;      
    else if(direction == 2) // Left
      v->x -= ai->moveSpeed;
    else if(direction == 3) // Up
      v->y -= ai->moveSpeed;
    else if(direction == 4) // Right
      v->x += ai->moveSpeed;

    ai->moveDuration = rand() % ai->moveMax + 5;
    ai->moveCount = 1;
  }  
  else
  {
    if(ai->moveDuration == ai->moveCount)
    {
      v->x = 0;
      v->y = 0;
      ai->moveCount = 0;
    } 
    else
    {
      if((p->x < 10 && v->x < 0) || (p->x > WINDOW_WIDTH - (10 + p->w) && v->x > 0))
        v->x = 0;
      else if(((p->y > WINDOW_HEIGHT - 200) && v->y > 0) || (p->y < 10 && v->y < 0))
        v->y = 0;

      ai->moveCount++;
    }
  }

  for(unsigned int e2 = 0; e2 < ENTITY_COUNT; ++e2)
  {
    if(e2 == entity)
      continue;
    if(!(world->mask[e2][COMPONENT_AI] & COMPONENT_ENABLED))
      continue;


    Position *p2 = &(world->position[e2]);

    float bottom1 = (p->y + v->y) + p->h;
    float bottom2 = p2->y + p2->h;
    float top1 = p->y + v->y;
    float top2 = p2->y;
    float left1 = p->x + v->x;
    float left2 = p2->x;
    float right1 = (p->x + v->x) + p->w;
    float right2 = p2->x + p2->w;

    if (bottom1 < top2) continue;
    if (top1 > bottom2) continue;
    if (right1 < left2) continue;
    if (left1 > right2) continue;

    v->x = 0;
    v->y = 0;
    break;
  }
}
