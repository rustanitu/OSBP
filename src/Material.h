#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm\vec3.hpp>

class Material
{
  Material();

  void SetAmbient(glm::vec3 amb);
  void SetDiffuse(glm::vec3 diff);
  void SetSpecular(glm::vec3 spec);
  void SetShiness(float shi);
  glm::vec3 GetAmbient();
  glm::vec3 GetDiffuse();
  glm::vec3 GetSpecular();
  float GetShiness();

private:
  glm::vec3 m_amb;
  glm::vec3 m_diff;
  glm::vec3 m_spec;
  float m_shiness;
};

#endif