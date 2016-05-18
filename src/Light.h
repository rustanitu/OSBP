#ifndef LIGHT_H
#define LIGHT_H

#include "ShaderObject.h"
#include <glm\vec3.hpp>

class Light : public ShaderObject
{
public:
  Light(glm::vec3 pos);
  void SetSpotLight(glm::vec3 target, float angle);
  
  void SetPosition(glm::vec3 pos);
  void SetAmbientLightColor(glm::vec3 color);
  void SetLightColor(glm::vec3 color);
  glm::vec3 GetPosition();
  glm::vec3 GetAmbientLightColor();
  glm::vec3 GetLightColor();
  
private:
  glm::vec3 m_pos;
  glm::vec3 m_amb;        // Ambient global light color
  glm::vec3 m_diffspec;   // This light color
  
  bool m_spot;
  glm::vec3 m_target;
  float m_angle;
};

#endif