#include "ShaderObject.h"


ShaderObject::ShaderObject()
{
}

void ShaderObject::Init()
{
  glGenVertexArrays(1, &m_vao);
  m_manip = new Manipulator(&m_model);
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
}

void ShaderObject::Draw()
{
}

void ShaderObject::EndDraw()
{
}
