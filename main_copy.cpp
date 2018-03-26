#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>

#include "GLOBALS.hpp"
#include "GBuffer.hpp"
#include "PingPongBuffer.hpp"
#include "Shader.hpp"

const char* f_vs = "first_vs.glsl";
const char* f_fs = "first_fs.glsl";

const char* a_cs = "2x1D_cs.glsl";
const char* b_cs = "10x10_cs.glsl";

const char* s_vs = "second_vs.glsl";
const char* s_fs = "second_fs.glsl";

void CreateTri(GLuint* vao_ptr, GLuint* vbo_ptr);
void CreateQuad(GLuint* vao_ptr, GLuint* vbo_ptr);
void Render(GLuint* vao_ptr, int n_vertices);

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

  //Generate vao and vbo for triangle...
  GLuint t_vao, t_vbo;
  CreateTri(&t_vao, &t_vbo);
  //... and for quad
  GLuint q_vao, q_vbo;
  CreateQuad(&q_vao, &q_vbo);

  //----------------------------------------------------------------------------

  GBuffer gBuffer;

  Shader firstShader(f_vs, f_fs);
  Shader secondShader(s_vs, s_fs);
  Shader computeShader(a_cs);

  //WIP: Adding either of the lines below black screens us

  //GLint xy_uni = computeShader.GetUniform("xORy");
  //PingPongBuffer ppBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, xy_uni);

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
    Render(&t_vao, 3);
    //Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //LGT PASS------------------------------------------------------------------
    glUseProgram(secondShader.GetProgram());
    //Bind texture
    gBuffer.PrepLightPass();
    //Draw
    Render(&q_vao, 4);

    //Window update-------------------------------------------------------------
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  //glDisableVertexAttribArray(0);  //NTS: ?
  return 0;
}

void CreateTri(GLuint* vao_ptr, GLuint* vbo_ptr) {
  // Hello triangle
  float trianglePoints[] = {
    -1.0f, -1.0f, 0.0f,   0.3f, 1.0f, 0.9f,
     1.0f, -1.0f, 0.0f,   0.6f, 1.0f, 0.6f,
     0.0f,  1.0f, 0.0f,   0.9f, 1.0f, 0.3f
  };


  glGenVertexArrays(1, vao_ptr);
  glGenBuffers(1, vao_ptr);

  // Bind them
  glBindVertexArray(*vao_ptr);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo_ptr);

  // Tell GL how to handle buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints), &trianglePoints, GL_STATIC_DRAW);

  // Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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
