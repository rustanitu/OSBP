#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include <glm\vec3.hpp>

class Material
{
public:
  Material();

  void SetAmbient(glm::vec3 color);

  void SetDiffuse(glm::vec3 color);

  void SetSpecular(glm::vec3 color);

  void SetShiness(float shi);

  void UseMaterial(ShaderProgram* shader);

private:
  glm::vec3 m_amb;
  glm::vec3 m_diff;
  glm::vec3 m_spec;
  float m_shi;
};

#endif