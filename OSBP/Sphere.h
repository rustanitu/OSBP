#ifndef SPHERE_H
#define SPHERE_H

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderObject.h"

class Sphere : public ShaderObject
{
public:
	Sphere(float r = 1, int nsl = 64, int nst = 32);

  void TransferData();
	void Draw();
	void DrawWire();
  void EndDraw() {}

private:
	float *m_vertices;
	int m_size;

};

#endif