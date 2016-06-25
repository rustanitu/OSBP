#include "ShaderObject.h"

#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>


ShaderObject::ShaderObject()
{
}

void ShaderObject::Init(ShaderProgram* shader, Camera* cam)
{
  glGenVertexArrays(1, &m_vao);
  m_shader = shader;
  m_manip = new Manipulator(&m_model);
  m_manip->SetCamera(cam);
  m_cam = cam;
}

ShaderObject::~ShaderObject()
{
  glDeleteVertexArrays(1, &m_vao);
  for (std::map<std::string, GLuint>::iterator it = m_buffers.begin(); it != m_buffers.end(); ++it)
  {
    glDeleteBuffers(1, &(*it).second);
  }
}

void ShaderObject::SetAttribute(std::string name, GLuint location, GLenum bufferType)
{
  m_attributes.insert(std::make_pair(name, location));
  glBindAttribLocation(m_shader->GetId(), location, name.c_str());

  GLuint id;
  glGenBuffers(1, &id);
  m_buffers.insert(std::make_pair(name, id));
  m_bufferTypes.insert(std::make_pair(name, bufferType));
}

void ShaderObject::InitDraw()
{
  GLint current;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current);

  if (m_shader->m_id != current)
    m_shader->UseProgram();

  if (m_material)
    m_material->UseMaterial(m_shader);
}

void ShaderObject::Draw(glm::mat4& inh_model)
{
  glm::mat4 vp = m_cam->m_proj * m_cam->m_view;
  glm::mat4 m = inh_model * GetModel();
  glm::mat4 mvp = vp * m;
  m_shader->SetUniform("mvp", mvp);
  m_shader->SetUniform("m", m);
  m_shader->SetUniform("_m", glm::transpose(glm::inverse(m)));
}

void ShaderObject::EndDraw()
{
}

void ShaderObject::Translate(float tx, float ty, float tz)
{
  m_model = glm::translate(m_model, glm::vec3(tx, ty, tz));
}

void ShaderObject::Rotate(float angle, float rx, float ry, float rz)
{
  m_model = glm::rotate(m_model, angle, glm::vec3(rx, ry, rz));
}

void ShaderObject::Scale(float sx, float sy, float sz)
{
  m_model = glm::scale(m_model, glm::vec3(sx, sy, sz));
}
