#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle {
private:
  GLuint m_vao, m_vbo;

public:
  Triangle() {
    float trianglePoints[] = {
      -1.0f, -1.0f, 0.0f,   0.3f, 1.0f, 0.9f,
       1.0f, -1.0f, 0.0f,   0.6f, 1.0f, 0.6f,
       0.0f,  1.0f, 0.0f,   0.9f, 1.0f, 0.3f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    // Bind them
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Tell GL how to handle buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePoints), &trianglePoints, GL_STATIC_DRAW);

    // Tell GL how to read buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  }

  void Render() {
    glBindVertexArray(m_vao);               //Bind the quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);  //Draw the stuff
    glBindVertexArray(0);                   //Unbind the quad
  }

  virtual ~Triangle () {};
};

#endif
