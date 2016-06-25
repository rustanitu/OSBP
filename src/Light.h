#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm\vec3.hpp>

class ShaderProgram;
class Light;

struct _light
{
  glm::vec3 pos;
  glm::vec3 dir;
  glm::vec3 amb;
  glm::vec3 diff;
  glm::vec3 spec;
  float shi;
  float spot;
};

class Light
{
public:
  // create m_light source specifying its m_position
  Light();

  void SetPosition(glm::vec3 pos);

  void SetDirection(glm::vec3 dir);

  void SetAmbient(glm::vec3 color);

  void SetDiffuse(glm::vec3 color);

  void SetSpecular(glm::vec3 color);

  void SetShiness(float shi);

  void SetSpot(float a);

  static void Setup(ShaderProgram* shader);

  _light GetLight()
  {
    return m_light;
  }

private:
  _light m_light;
  static std::vector<Light*> s_lights;
};

#endif