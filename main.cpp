#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 400

// TODO Make hello triangle
int main() {
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

  // Hello triangle
  float trianglePoints[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

  // Generate vao and vbo
  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // Bind them
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Tell GL how to handle buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints), &trianglePoints, GL_STATIC_DRAW);

  // Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // MAIN LOOP
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Draw
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
glfwWindowShouldClose(window) == 0);

  glDisableVertexAttribArray(0);
  return 0;
}
