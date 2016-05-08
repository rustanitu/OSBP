#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "ShaderProgram.h"

#include <GL\glew.h>
#include <map>

#include "Manipulator.h"

class ShaderObject
{
public:
  ShaderObject();

  void Init(ShaderProgram* shader);

  ~ShaderObject();

  void SetAttribute(std::string, GLuint location, GLenum bufferType);

  virtual void InitDraw() = 0;

  virtual void Draw() = 0;

  virtual void EndDraw() = 0;

  void SetManipulatorCamera(Camera* cam)
  {
    m_manip->SetCamera(cam);
  }

  Manipulator* GetManipulator()
  {
    return m_manip;
  }

  glm::mat4 GetModel()
  {
    return m_model;
  }

protected:
  GLuint m_vao;
  ShaderProgram* m_shader;
  std::map<std::string, GLuint> m_buffers;
  std::map<std::string, GLenum> m_bufferTypes;
  std::map<std::string, GLuint> m_attributes;

private:
  Manipulator* m_manip;
  glm::mat4 m_model;
};

#endif