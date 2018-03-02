#ifndef GBUFFER_H
#define GBUFFER_H

#include <GL/glew.h>

class GBuffer
{
private:
  GLuint m_gBuffer;
  GLuint m_gColour;

  GLuint m_rbo_depth;
  GLuint m_uni_gColour;

  void CreateTexture(GLuint id, GLint internalFormat, GLenum format, GLenum type);
  void AttachTexture(GLuint id, GLenum attachment);
public:
  GBuffer();
  ~GBuffer();

  void PrepGeoPass();
  void PrepLightPass();

  void FindUniformSamplerLoc(GLuint shader_program_id);
  void UploadUniformSamplers();

  GLuint GetColTextureId();
};

#endif
