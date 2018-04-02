#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H
#include "Shader.hpp"

#include <GL/glew.h>

class PingPongBuffer
{
private:
  GLuint m_buffers[2];
  int m_texture_width;
  int m_texture_height;

  Shader* m_computeShaderPtrs[2];

  void createBuffer(GLuint buffer_id);
  void bindAndCompute(GLuint source_buffer, GLuint target_buffer, GLuint x, GLuint y);
public:
  PingPongBuffer(int texture_width, int texture_height, Shader* computeShaderPtr_x, Shader* computeShader_y);
  ~PingPongBuffer();

  void DoPingPong(int n_passes, GLuint src_buffer);

  void BindResult();
};

#endif
