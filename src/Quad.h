#ifndef Quad_H
#define Quad_H

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderObject.h"

class Quad : public ShaderObject
{
public:
	Quad(float l = 2);
  ~Quad();

  void TransferData();
  void ConnectBuffer();
  void InitDraw() {}
  void Draw(glm::mat4& inh_model = glm::mat4());
  void EndDraw() {}

  void SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType);

  void SetNormalAttribute(std::string attname, GLuint location, GLenum bufferType);

  void SetTextureAttribute(std::string attname, GLuint location, GLenum bufferType);

  void SetTangentAttribute(std::string attname, GLuint location, GLenum bufferType);

  void SetBitangentAttribute(std::string attname, GLuint location, GLenum bufferType);

private:
	float *m_vertices;
  float *m_texcoord;

  std::string m_vertex;
  std::string m_normal;
  std::string m_texture;

	int m_size;
};

#endif