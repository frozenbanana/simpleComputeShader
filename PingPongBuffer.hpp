#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H

#include <GL/glew.h>

class PingPongBuffer
{
private:
  GLuint m_buffers[2];
  int m_texture_width;
  int m_texture_height;

  //GLint m_xy_uniLoc;

  void createBuffer(GLuint buffer_id);
  void bindAndCompute(GLuint source_buffer, GLuint target_buffer);
public:
  PingPongBuffer(int texture_width, int texture_height);//, GLint xy_uniform_id);
  ~PingPongBuffer();

  void DoPingPong(int n_passes, GLuint src_buffer);

  void BindResult();
};

#endif
