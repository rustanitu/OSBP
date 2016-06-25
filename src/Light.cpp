#include"Light.h"
#include "ShaderProgram.h"

std::vector<Light*> Light::s_lights;

Light::Light()
{
  Light::s_lights.push_back(this);
}

void Light::SetPosition(glm::vec3 pos)
{
  m_light.pos = pos;
}

void Light::SetDirection(glm::vec3 dir)
{
  m_light.dir = dir;
}

// Set ambient contribution
void Light::SetAmbient(glm::vec3 color)
{
  m_light.amb = color;
}

// Set diffuse contribution
void Light::SetDiffuse(glm::vec3 color)
{
  m_light.diff = color;
}

// Set specular contributiuon
void Light::SetSpecular(glm::vec3 color)
{
  m_light.spec = color;
}

void Light::SetSpot(float a)
{
  m_light.spot = a;
}

void Light::SetShiness(float shi)
{
  m_light.shi = shi;
}

// load m_light.light source (turn it on)
void Light::Setup(ShaderProgram* shader)
{
  int size = s_lights.size();
  //_light* lights = new _light[size];

  glm::vec3* pos = new glm::vec3[size];
  glm::vec3* dir = new glm::vec3[size];
  glm::vec3* amb = new glm::vec3[size];
  glm::vec3* diff = new glm::vec3[size];
  glm::vec3* spec = new glm::vec3[size];
  float* shi = new float[size];
  float* spot = new float[size];

  for (int i = 0; i < size; i++)
  {
    //lights[i] = s_lights[i]->GetLight();
    _light light = s_lights[i]->GetLight();
    
    pos[i] = light.pos;
    dir[i] = light.dir;
    amb[i] = light.amb;
    diff[i] = light.diff;
    spec[i] = light.spec;
    shi[i] = light.shi;
    spot[i] = light.spot;
  }

  shader->SetUniform("lpos", size, pos);
  shader->SetUniform("ldir", size, dir);
  shader->SetUniform("lamb", size, amb);
  shader->SetUniform("ldiff", size, diff);
  shader->SetUniform("lspec", size, spec);
  shader->SetUniform("lshi", size, shi);
  shader->SetUniform("lspot", size, spot);
}