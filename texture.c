#include "texture.h"

#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <stdio.h>

GLuint loadTexture(char* filename)
{
  SDL_Surface *surface;
  GLuint textureid;
  int mode;

  surface = IMG_Load(filename);

  if (!surface) {
    return 0;
  }

  // work out what format to tell glTexImage2D to use...
  if (surface->format->BytesPerPixel == 3) // RGB 24bit
    mode = GL_RGB;
  else if (surface->format->BytesPerPixel == 4) // RGBA 32bit
    mode = GL_RGBA;
  else
  {
    SDL_FreeSurface(surface);
    return 0;
  }

  glGenTextures(1, &textureid);
  glBindTexture(GL_TEXTURE_2D, textureid);
  glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  SDL_FreeSurface(surface);

  return textureid;
}
