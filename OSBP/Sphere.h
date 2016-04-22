#ifndef SPHERE_H
#define SPHERE_H

#include <GL\glew.h>
#include <GL\freeglut.h>

class Sphere
{
public:
	Sphere(float r = 1, int nsl = 64, int nst = 32);

	void Draw();
  void DrawWire();

  void SetVerticesAttribute(GLuint id)
  {
    m_vid = id;
  }

  void SetColorsAttribute(GLuint id)
  {
    m_cid = id;
  }

private:
  float *m_vertices;
  int m_size;
  int m_vid, m_cid;

};

#endif