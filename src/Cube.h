#ifndef CUBE_H
#define CUBE_H

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderObject.h"

class Cube : public ShaderObject
{
public:
	Cube(float l = 2);
	~Cube();

	void TransferData();
	void ConnectBuffer();
	void InitDraw() {}
	void Draw(glm::mat4& inh_model = glm::mat4());
	void EndDraw() {}

	void SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType);

private:
	float *m_vertices;

	std::string m_vertex;

	int m_size;
};

#endif