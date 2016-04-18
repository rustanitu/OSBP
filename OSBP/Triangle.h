#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <GL\glew.h>

static float coord[] = {
 -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  0.0f,  1.0f, -1.0f,
};

static float color[] = {
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
};

class Triangle
{
public:
  Triangle() {}
  ~Triangle() {}

  void SetVerticesAttribute(GLuint id)
  {
    v_att_id = id;
  }

  void SetColorsAttribute(GLuint id)
  {
    c_att_id = id;
  }

  void Draw()
  {
    glVertexAttribPointer(v_att_id, 3, GL_FLOAT, GL_FALSE, 0, coord);
    glEnableVertexAttribArray(v_att_id);
    glVertexAttribPointer(c_att_id, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(c_att_id);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  GLuint v_att_id;
  GLuint c_att_id;
};

#endif