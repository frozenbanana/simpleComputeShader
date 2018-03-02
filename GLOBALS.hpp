#ifndef GLOBALS_H
#define GLOBALS_H

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 400


#include <GL/glew.h>

//#include <stdio.h>

enum TextureEnums {
  GBUF_TEX,        //0
  COMP_TEX,        //1
  NUM_OF_TEX
};

inline void Bind2DTextureTo(GLuint texture_id, TextureEnums target_unit) {
  //std::cout << "Binding texture id: " << texture_id << " to texture unit: " << target_unit << '\n';
  glActiveTexture(GL_TEXTURE0 + target_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

#endif
