#include "Shader.hpp"

#include <iostream>
#include <string>

#include <fstream>
#include <streambuf>
#include <string> //to_string();

#include "GLOBALS.hpp"

//Private

void Shader::MakeShader(GLuint id, const char* path) {

  //----------------------------------------------------------------------------
  std::ifstream glsl_file(path);

  if (!(glsl_file.is_open())) {
    std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << path << std::endl;
  }

  std::string glsl_text((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
  glsl_file.close();

  const char* glsl_data = glsl_text.c_str();
  //----------------------------------------------------------------------------

  glShaderSource(id, 1, &glsl_data, NULL);
  glCompileShader(id);

  // Check if shader where compiled fine
  int  success;
  char infoLog[512];

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if(!success) {
      GLint type;
      glGetShaderiv(id, GL_SHADER_TYPE, &type);
      std::string type_name = "";

      switch (type) {
        case GL_VERTEX_SHADER:
          type_name = "VERTEX";
          break;
        case GL_GEOMETRY_SHADER:
          type_name = "GEOMETRY";
          break;
        case GL_FRAGMENT_SHADER:
          type_name = "FRAGMENT";
          break;
        case GL_COMPUTE_SHADER:
          type_name = "COMPUTE";
          break;
        default:
          type_name = "UNKNOWN";
          break;
      }

      glGetShaderInfoLog(id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::" << type_name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

void Shader::LinkProgram() {

  glLinkProgram(this->m_program);

  int  success;
  char infoLog[512];

  glGetProgramiv(this->m_program, GL_LINK_STATUS, &success);
  if(!success) {
      glGetShaderInfoLog(this->m_program, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
}

//Public
Shader::Shader(const char* cs_path) {


  GLuint cs_id = glCreateShader(GL_COMPUTE_SHADER);
  this->MakeShader(cs_id, cs_path);

  this->m_program = glCreateProgram();
  glAttachShader(this->m_program, cs_id);

  this->LinkProgram();
}

Shader::Shader(const char* vs_path, const char* fs_path) {
  GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
  this->MakeShader(vs_id, vs_path);

  GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
  this->MakeShader(fs_id, fs_path);

  this->m_program = glCreateProgram();
  glAttachShader(this->m_program, vs_id);
  glAttachShader(this->m_program, fs_id);
  this->LinkProgram();
}

Shader::~Shader() {
}

GLuint Shader::GetProgram(){
  return this->m_program;
}

GLint Shader::GetUniform(std::string uniform_name) {
  //Function general;

  printOpenGLError(); //NO ERROR //ALT: If this is done on the compute shader it gets an INVALID OPERATION HERE

  GLint uniLoc = glGetUniformLocation(this->m_program, uniform_name.c_str());

  printOpenGLError(); //NO ERROR

  if(uniLoc == (GLint)-1) {
    //If it does not, write out an error
    std::cout << "ERROR::SHADER::" << uniform_name << "::UNIFORM_NOT_FOUND" << std::endl;
  }

  printOpenGLError(); // NO ERROR

  return uniLoc;
}
