#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <GL\glew.h>
#include <map>

#include "ShaderProgram.h"

class ShaderObject
{
public:
  ShaderObject();

  void Init();

  ~ShaderObject();

  void SetAttribute(std::string, GLuint location, GLenum bufferType);

  virtual void InitDraw();

  virtual void Draw();

  virtual void EndDraw();

protected:
  GLuint m_vao; 
  ShaderProgram m_shader;
  std::map<std::string, GLuint> m_buffers;
  std::map<std::string, GLenum> m_bufferTypes;
  std::map<std::string, GLuint> m_attributes;
};

#endif