#ifndef SHADER_H
#define SHADER_H
//#include <iostream>
//#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
  GLuint m_program;

  void MakeShader(GLuint id, const char* path);
  void LinkProgram();

public:
  Shader(const char* cs_path);
  Shader(const char* vs_path, const char* fs_path);
  ~Shader();

  GLuint GetProgram();

};
#endif  //SHADER_H
