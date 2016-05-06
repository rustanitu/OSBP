#ifndef SPHERE_H
#define SPHERE_H

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderObject.h"

struct VertexInfo;

class Sphere : public ShaderObject
{
public:
	Sphere(float r = 1, int nsl = 64, int nst = 32);
  ~Sphere();

  glm::vec4 SolvePlan(const VertexInfo& v0, const VertexInfo& v1, const VertexInfo& v2);
  void SetTangentVectors(VertexInfo& v0, VertexInfo& v01, VertexInfo& v2);

  void TransferData();
  void ConnectBuffer();
  void InitDraw() {}
	void Draw();
  void EndDraw() {}

private:
	float *m_vertices;
  float *m_texcoord;
  float *m_tangents;
  float *m_bitangents;
	int m_size;

};

#endif