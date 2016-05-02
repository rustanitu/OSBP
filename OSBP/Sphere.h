#ifndef SPHERE_H
#define SPHERE_H

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderObject.h"

class Sphere : public ShaderObject
{
public:
	Sphere(float r = 1, int nsl = 64, int nst = 32);
  ~Sphere();

  void TransferData();
  void InitDraw() {}
	void Draw();
	void DrawWire();
  void EndDraw() {}

private:
	float *m_vertices;
  float *m_texcoord;
	int m_size;

};

#endif