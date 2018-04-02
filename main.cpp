#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>

#include "GLOBALS.hpp"
#include "GBuffer.hpp"
#include "PingPongBuffer.hpp"
#include "Shader.hpp"
#include "Triangle.hpp"
#include "Quad.hpp"

const char* f_vs = "shaders/first_vs.glsl";
const char* f_fs = "shaders/first_fs.glsl";
const char* a_cs = "shaders/2x1D_cs.glsl";      // broken
const char* b_cs = "shaders/10x10_cs.glsl";     // broken
const char* c_cs = "shaders/PT_cs.glsl";        // makes red color. it works
const char* d_cs = "shaders/frozen_cs_x.glsl";    // experimental
const char* e_cs = "shaders/frozen_cs_y.glsl";    // experimental
const char* s_vs = "shaders/second_vs.glsl";
const char* s_fs = "shaders/second_fs.glsl";


#define NR_OF_COMPUTE_PASSES 1

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

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "ERROR: Failed to initialize GLEW" << std::endl;
    return 0;
  }
  //Error: Invalid Enum (Last and potentially only relevant error from search)

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "Version: " << version << std::endl;
  std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

  //----------------------------------------------------------------------------
  Triangle triangle;
  Quad quad;
  //----------------------------------------------------------------------------

  GBuffer gBuffer;

  Shader firstShader(f_vs, f_fs);
  Shader secondShader(s_vs, s_fs);
  Shader computeShader_x(d_cs);
  Shader computeShader_y(e_cs);

  //WIP: Adding either of the lines below black screens us
  PingPongBuffer ppBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, &computeShader_x, &computeShader_y);

  //Lgt pass setup
  glUseProgram(secondShader.GetProgram());
  gBuffer.FindUniformSamplerLoc(secondShader.GetProgram());
  gBuffer.UploadUniformSamplers();    //NTS: Move to loop?

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
    triangle.Render();
    //Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //COMPUTE PASS--------------------------------------------------------------
    ppBuffer.DoPingPong(NR_OF_COMPUTE_PASSES, gBuffer.GetColTextureId());

    //LGT PASS------------------------------------------------------------------
    glUseProgram(secondShader.GetProgram());
    //Bind texture
    gBuffer.PrepLightPass();
    ppBuffer.BindResult();
    //Draw
    quad.Render();
    //Window update-------------------------------------------------------------
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  //glDisableVertexAttribArray(0);  //NTS: ?
  return 0;
}
