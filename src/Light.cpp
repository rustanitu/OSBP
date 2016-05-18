#include "Light.h"

Light::Light(glm::vec3 pos)
{
  m_pos = pos;
}

void Light::SetSpotLight(glm::vec3 target, float angle)
{
  m_spot = true;
  m_target = target;
  m_angle = angle;
}

void Light::SetPosition(glm::vec3 pos)
{
  m_pos = pos;
}

void Light::SetAmbientLightColor(glm::vec3 color)
{
  m_amb = color;
}

void Light::SetLightColor(glm::vec3 color)
{
  m_diffspec = color;
}

glm::vec3 Light::GetPosition()
{
  return m_pos;
}

glm::vec3 Light::GetAmbientLightColor()
{
  return m_amb;
}

glm::vec3 Light::GetLightColor()
{
  return m_diffspec;
}
