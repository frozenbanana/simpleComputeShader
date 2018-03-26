#include "PingPongBuffer.hpp"

#include "GLOBALS.hpp"

//Private
void PingPongBuffer::createBuffer(GLuint buffer_id) {
  //Bind2DTextureTo(buffer_id, COMPUTE_TEX);
  //glBindTexture(GL_TEXTURE_2D, buffer_id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,				          //Mipmaps
		GL_RGBA,		        //Internal format		//GL_RGB16F
    this->m_texture_width,
    this->m_texture_height,
		0,				          //Frontier stuff
		GL_RGBA,		        //Format read			//GL_RGB
		GL_UNSIGNED_BYTE,		//Type of values in read format		//GL_FLOAT
		NULL			          //source
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//When shrunk go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//When enlarged go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge

  //glBindTexture(GL_TEXTURE_2D, 0);
}

void PingPongBuffer::bindAndCompute(GLuint source_buffer, GLuint target_buffer) {
  glBindImageTexture(
    0,                  //Always bind to slot 0
    source_buffer,
    0,
    GL_FALSE,
    0,
    GL_READ_ONLY,			  //Only read from this texture
    GL_RGBA8 //GL_RGBA32F						//GL_RGB16F
  );

  glBindImageTexture(
    1,                  //Always bind to slot 1
    target_buffer,
    0,
    GL_FALSE,
    0,
    GL_WRITE_ONLY,			//Only write to this texture
    GL_RGBA8 //GL_RGBA32F						//GL_RGB16F
  );

  glDispatchCompute(this->m_texture_width / 10, this->m_texture_height / 10, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

//Public
PingPongBuffer::PingPongBuffer(int texture_width, int texture_height) {//, GLint xy_uniform_id) {
  this->m_texture_width = texture_width;
  this->m_texture_height = texture_height;

  //this->m_xy_uniLoc = xy_uniform_id;

  glGenTextures(2, this->m_buffers);

  glBindTexture(GL_TEXTURE_2D, this->m_buffers[0]);
  this->createBuffer(this->m_buffers[0]);
  glBindTexture(GL_TEXTURE_2D, this->m_buffers[1]);
  this->createBuffer(this->m_buffers[1]);
}

PingPongBuffer::~PingPongBuffer() {

}

void PingPongBuffer::DoPingPong(int n_passes, GLuint src_buffer) {

  int n = n_passes * 2;			//Each iteration of the blur includes both x and y axis
                            //Also ensures that pingpongBuffer[0] is always written to last
  int x = 1;
  int y = 0;

  //Do a first pass if there are supposed to be passes at all
  if(n_passes > 0){

    //glUniform2i(this->m_xy_uniLoc, y, x);		                                    //Update uniform vector

    this->bindAndCompute(src_buffer, this->m_buffers[1]);
  }

  for (int i = 1; i < n; i++) {								                                  //Loop starts at 1 as the first pass has been done

    //glUniform2i(this->m_xy_uniLoc, x, y);		                                    //Update uniform vector

    this->bindAndCompute(this->m_buffers[x], this->m_buffers[y]);	              //Send in alternating buffers

    //Swap so x = 0 or 1
    //and y = 1 or 0
    x = !x;
    y = !y;

  }
}

void PingPongBuffer::BindResult() {
  Bind2DTextureTo(this->m_buffers[0], COMP_TEX);
  //glBindImageTexture(
  //  COMP_TEX,        //Slot 1
  //  this->m_buffers[0],
  //  0,
  //  GL_FALSE,
  //  0,
  //  GL_READ_WRITE,			  //Only read from this texture
  //  GL_RGBA8						//GL_RGB16F
  //);
}
