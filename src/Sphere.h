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

  void SetTangentVectors(VertexInfo& v0, VertexInfo& v01, VertexInfo& v2);

  void TransferData();
  void ConnectBuffer();
  void InitDraw() {}
	void Draw();
  void EndDraw() {}

  void SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType);
  void SetNormalAttribute(std::string attname, GLuint location, GLenum bufferType);
  void SetTextureAttribute(std::string attname, GLuint location, GLenum bufferType);
  void SetTangentAttribute(std::string attname, GLuint location, GLenum bufferType);
  void SetBitangentAttribute(std::string attname, GLuint location, GLenum bufferType);

private:
	float *m_vertices;
  float *m_texcoord;
  float *m_tangents;
  float *m_bitangents;

  std::string m_vertex;
  std::string m_normal;
  std::string m_texture;
  std::string m_tangent;
  std::string m_bitangent;

	int m_size;

};

#endif