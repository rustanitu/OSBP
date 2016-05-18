#include "Material.h"

void Material::SetAmbient(glm::vec3 amb)
{
  m_amb = amb;
}

void Material::SetDiffuse(glm::vec3 diff)
{
  m_diff = diff;
}

void Material::SetSpecular(glm::vec3 spec)
{
  m_spec = spec;
}


void Material::SetShiness(float shi)
{
  m_shiness = shi;
}

glm::vec3 Material::GetAmbient()
{
  return m_amb;
}

glm::vec3 Material::GetDiffuse()
{
  return m_diff;
}

glm::vec3 Material::GetSpecular()
{
  return m_spec;
}


float Material::GetShiness()
{
  return m_shiness;
}