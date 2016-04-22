#ifndef SPHERE_H
#define SPHERE_H

#include<stdlib.h>
#include<math.h>

#include"Material.h"
#include"Face.h"
#include"Shape.h"

#include<GL\glut.h>

class Sphere : public Shape
{
private:
	float radius;
	Face** face;
	int total;

public:
	Sphere(float r = 1, int nsl = 64, int nst = 32);

	void draw(unsigned int primitive = GL_TRIANGLES);
};

#endif