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

//-----------------------------------------------------------------
// Print for OpenGL errors
//
// Returns 1 if an OpenGL error occurred, 0 otherwise.
//
//Source: http://www.lighthouse3d.com/cg-topics/error-tracking-in-opengl/

// #define printOpenGLError() printOglError(__FILE__, __LINE__)
//
// inline int printOglError(char *file, int line)
// {
//
//     GLenum glErr;
//     int    retCode = 0;
//
//     glErr = glGetError();
//     if (glErr != GL_NO_ERROR)
//     {
//         printf("glError in file %s @ line %d: %s\n",
// 			     file, line, gluErrorString(glErr));
//         retCode = 1;
//     }
//     return retCode;
// }

#endif
