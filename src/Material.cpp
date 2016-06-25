#include"Material.h"

Material::Material()
{

}

void Material::SetAmbient(glm::vec3 color)
{
  m_amb = color;
}

void Material::SetDiffuse(glm::vec3 color)
{
  m_diff = color;
}

void Material::SetSpecular(glm::vec3 color)
{
  m_spec = color;
}

void Material::SetShiness(float shi)
{
  m_shi = shi;
}

void Material::UseMaterial(ShaderProgram* shader)
{
  shader->SetUniform("mamb", m_amb);
  shader->SetUniform("mdiff", m_diff);
  shader->SetUniform("mspec", m_spec);
  shader->SetUniform("mshi", m_shi);
}