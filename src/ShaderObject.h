#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "ShaderProgram.h"

#include <GL\glew.h>
#include <map>

#include "Material.h"
#include "Manipulator.h"

class ShaderObject
{
public:
  ShaderObject();

  void Init(ShaderProgram* shader, Camera* cam);

  ~ShaderObject();

  void SetAttribute(std::string, GLuint location, GLenum bufferType);

  virtual void InitDraw() = 0;

  virtual void Draw(glm::mat4& inh_model = glm::mat4()) = 0;

  virtual void EndDraw() = 0;

  Manipulator* GetManipulator()
  {
    return m_manip;
  }

  glm::mat4& GetModel()
  {
    return m_model;
  }

  void SetMaterial(Material* material)
  {
    m_material = material;
  }

  virtual void Translate(float tx, float ty, float tz);

  virtual void Rotate(float angle, float rx, float ry, float rz);

  virtual void Scale(float sx, float sy, float sz);

protected:
  GLuint m_vao;
  ShaderProgram* m_shader;
  std::map<std::string, GLuint> m_buffers;
  std::map<std::string, GLenum> m_bufferTypes;
  std::map<std::string, GLuint> m_attributes;

private:
  Camera* m_cam;
  Manipulator* m_manip;
  glm::mat4 m_model;
  Material* m_material;
};

#endif