#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>

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
  GLint GetUniform(std::string uniform_name);

};
#endif  //SHADER_H
