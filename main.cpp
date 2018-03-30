#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>

#include "GLOBALS.hpp"
#include "GBuffer.hpp"
//#include "PingPongBuffer.hpp"
#include "Shader.hpp"

const char* f_vs = "first_vs.glsl";
const char* f_fs = "first_fs.glsl";

const char* a_cs = "2x1D_cs.glsl";
const char* b_cs = "10x10_cs.glsl";
const char* c_cs = "PT_cs.glsl";

const char* s_vs = "second_vs.glsl";
const char* s_fs = "second_fs.glsl";

void CreateTri(GLuint* vao_ptr, GLuint* vbo_ptr);
void CreateQuad(GLuint* vao_ptr, GLuint* vbo_ptr);
void Render(GLuint* vao_ptr, int n_vertices);

//##############################################################################
void DoPingPong(int n_passes, GLuint src_buffer, GLuint buffer_arr[2]);
void BindAndCompute(GLuint source_buffer, GLuint target_buffer);
//##############################################################################

int main() {

  //----------------------------------------------------------------------------

  // GLFW
  glfwInit();
  if(!glfwInit()) {
    std::cout << "ERROR: Failed to initialize GLFW" << std::endl;
    return 0;
  }

  // Create window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Compute Shader", NULL, NULL);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  if (!window) {
    std::cout << "ERROR: could not open window with GLFW3" << std::endl;
    glfwTerminate();
    return 0;
  }

  //printOpenGLError(); //NO ERROR

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;

  //printOpenGLError(); //NO ERROR

  if (glewInit() != GLEW_OK) {
    std::cout << "ERROR: Failed to initialize GLEW" << std::endl;
    return 0;
  }

  printOpenGLError(); //INVALID ENUMERANT

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "Version: " << version << std::endl;
  std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;


  //printOpenGLError(); //NO ERROR
  //----------------------------------------------------------------------------

  //Generate vao and vbo for triangle...
  GLuint t_vao, t_vbo;
  CreateTri(&t_vao, &t_vbo);

  //printOpenGLError(); //NO ERROR

  //... and for quad
  GLuint q_vao, q_vbo;
  CreateQuad(&q_vao, &q_vbo);

  //printOpenGLError(); //NO ERROR
  //----------------------------------------------------------------------------

  GBuffer gBuffer;

  //printOpenGLError(); //NO ERROR

  Shader firstShader(f_vs, f_fs);
  Shader secondShader(s_vs, s_fs);
  Shader computeShader(c_cs);

  //printOpenGLError(); //NO ERROR

  //WIP: Adding either of the lines below black screens us
  //GLint xy_uni = computeShader.GetUniform("xORy");
  //PingPongBuffer ppBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);//, xy_uni);

  //############################################################################

  GLuint pp_buffers[2];

  glGenTextures(2, pp_buffers);

  //this->createBuffer(this->m_buffers[0]);
  glBindTexture(GL_TEXTURE_2D, pp_buffers[0]);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,				          //Mipmaps
    GL_RGBA,		        //Internal format
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0,				          //Frontier stuff
    GL_RGBA,		        //Format read
    GL_UNSIGNED_BYTE,		//Type of values in read format
    NULL			          //source
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//When shrunk go blurry
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//When enlarged go blurry
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge

  //this->createBuffer(this->m_buffers[1]);
  glBindTexture(GL_TEXTURE_2D, pp_buffers[1]);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,				          //Mipmaps
    GL_RGBA,		        //Internal format
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0,				          //Frontier stuff
    GL_RGBA,		        //Format read
    GL_UNSIGNED_BYTE,		//Type of values in read format
    NULL			          //source
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//When shrunk go blurry
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//When enlarged go blurry
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge

  //############################################################################

  //printOpenGLError(); //NO ERROR

  //compute pass setup
  //glUseProgram(computeShader.GetProgram());
  //GLint xy_uni = computeShader.GetUniform("xORy");

  printOpenGLError(); //INVALID OPERATION (Black screens us

  //Lgt pass setup
  glUseProgram(secondShader.GetProgram());
  gBuffer.FindUniformSamplerLoc(secondShader.GetProgram());
  gBuffer.UploadUniformSamplers();    //NTS: Move to loop?

  //printOpenGLError(); //NO ERROR

  //----------------------------------------------------------------------------

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // MAIN LOOP
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //GEO PASS------------------------------------------------------------------
    glUseProgram(firstShader.GetProgram());
    //Set framebuffer
    gBuffer.PrepGeoPass();
    //Draw
    Render(&t_vao, 3);
    //Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //printOpenGLError(); //NO ERROR

    //COMPUTE PASS--------------------------------------------------------------
    glUseProgram(computeShader.GetProgram());
    //ppBuffer.DoPingPong(1, gBuffer.GetColTextureId());
    DoPingPong(1, gBuffer.GetColTextureId(), pp_buffers);

    //printOpenGLError(); //NO ERROR (INVALID OPERATION caught internally : FIXED: now actually calls glUseProgram on the compute shader)

    //LGT PASS------------------------------------------------------------------
    glUseProgram(secondShader.GetProgram());
    //Bind texture
    gBuffer.PrepLightPass();
    //ppBuffer.BindResult();
    Bind2DTextureTo(pp_buffers[0], COMP_TEX);
    //Draw
    Render(&q_vao, 4);

    //printOpenGLError(); //NO ERROR

    //Window update-------------------------------------------------------------
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    //printOpenGLError(); //NO ERROR

    //while(true){};
  }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  printOpenGLError(); //End with calling this just to be sure nothing has been generated during run

  //glDisableVertexAttribArray(0);  //NTS: ?
  return 0;
}

void CreateTri(GLuint* vao_ptr, GLuint* vbo_ptr) {
  // Hello triangle
  float trianglePoints[] = {
    -1.0f, -1.0f, 0.0f,   0.3f, 0.0f, 0.9f,
     1.0f, -1.0f, 0.0f,   0.6f, 0.0f, 0.6f,
     0.0f,  1.0f, 0.0f,   0.9f, 0.0f, 0.3f
  };


  glGenVertexArrays(1, vao_ptr);
  glGenBuffers(1, vao_ptr);

  //printOpenGLError(); //NO ERROR

  // Bind them
  glBindVertexArray(*vao_ptr);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo_ptr);

  //printOpenGLError(); //NO ERROR

  // Tell GL how to handle buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints), &trianglePoints, GL_STATIC_DRAW);

  //printOpenGLError(); //NO ERROR

  // Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

  //printOpenGLError(); //NO ERROR

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  //printOpenGLError(); //NO ERROR
}

void CreateQuad(GLuint* vao_ptr, GLuint* vbo_ptr) {
  //Create four vertices covering the window
  float quad[] = {
    //Pos                 //UVs
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f
  };

  //Generate arrays and buffers
  glGenVertexArrays(1, vao_ptr);
  glGenBuffers(1, vbo_ptr);

  //Bind them together
  glBindVertexArray(*vao_ptr);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo_ptr);

  //Tell GL how to handle the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);

  //Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,                  //Index of the attribute
    3,                  //Number of components for the attribute
    GL_FLOAT,           //Type of values in attribute
    GL_FALSE,           //Are they normalized?
    5 * sizeof(float),  //In the array, how long steps should be taken for each element?
    (void*)0          //Inside the element, how big is the offset to the relevant values?
  );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Render(GLuint* vao_ptr, int n_vertices) {
  glBindVertexArray(*vao_ptr);     //Bind the quad
  glDrawArrays(GL_TRIANGLE_STRIP, 0, n_vertices);  //Draw the stuff
  glBindVertexArray(0);                   //Unbind the quad
}

//##############################################################################
void DoPingPong(int n_passes, GLuint src_buffer, GLuint buffer_arr[2]) {

  //printOpenGLError(); //NO ERROR

  int n = n_passes * 2;			//Each iteration of the blur includes both x and y axis
                            //Also ensures that pingpongBuffer[0] is always written to last
  int x = 1;
  int y = 0;

  //Do a first pass if there are supposed to be passes at all
  if(n_passes > 0){

    //glUniform2i(this->m_xy_uniLoc, y, x);		                                    //Update uniform vector

    BindAndCompute(src_buffer, buffer_arr[1]);
  }

  //printOpenGLError(); //NO ERROR

  for (int i = 1; i < n; i++) {								                                  //Loop starts at 1 as the first pass has been done

    //glUniform2i(this->m_xy_uniLoc, x, y);		                                    //Update uniform vector

    BindAndCompute(buffer_arr[x], buffer_arr[y]);	              //Send in alternating buffers

    //Swap so x = 0 or 1
    //and y = 1 or 0
    x = !x;
    y = !y;

  }

  //printOpenGLError(); //NO ERROR
}

void BindAndCompute(GLuint source_buffer, GLuint target_buffer) {

  //printOpenGLError(); //NO ERROR

  glBindImageTexture(
    0,                  //Always bind to slot 0
    source_buffer,
    0,
    GL_FALSE,
    0,
    GL_READ_ONLY,			  //Only read from this texture
    GL_RGBA8 //GL_RGBA32F						//GL_RGB16F
  );

  //printOpenGLError(); //NO ERROR

  glBindImageTexture(
    1,                  //Always bind to slot 1
    target_buffer,
    0,
    GL_FALSE,
    0,
    GL_WRITE_ONLY,			//Only write to this texture
    GL_RGBA8 //GL_RGBA32F						//GL_RGB16F
  );

  //printOpenGLError(); //NO ERROR

  glDispatchCompute((float)(WINDOW_WIDTH / 10), (float)(WINDOW_HEIGHT / 10), 1);

  //printOpenGLError(); //NO ERROR

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  //printOpenGLError(); //NO ERROR
}
