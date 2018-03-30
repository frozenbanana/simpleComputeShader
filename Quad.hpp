#ifndef QUAD_HPP
#define QUAD_HPP

class Quad {
private:
  GLuint m_vao, m_vbo;

public:
  Quad() {

  float quad[] = {
    //Pos                 //UVs
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f
  };

  //Generate arrays and buffers
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  //Bind them together
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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

  void Render() {
    glBindVertexArray(m_vao);               //Bind the quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  //Draw the stuff
    glBindVertexArray(0);                   //Unbind the quad
  }

  virtual ~Quad () {};
};

#endif
