#include"Quad.h"

Quad::Quad(float l)
{
  float d = l / 2.0f;

  float vertices[] = { -d, -d, 0.0f, d, -d, 0.0f, -d, d, 0.0f, -d, d, 0.0f, d, -d, 0.0f, d, d, 0.0f };
  float texcoord[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

  m_size = 6;
  m_vertices = new float[m_size * 3];
  m_texcoord = new float[m_size * 2];

  m_vertices = (float*)memcpy(m_vertices, vertices, m_size * 3 * sizeof(float));
  m_texcoord = (float*)memcpy(m_texcoord, texcoord, m_size * 2 * sizeof(float));
}

Quad::~Quad()
{
  delete[] m_vertices;
  delete[] m_texcoord;
}

void Quad::SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_vertex = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Quad::SetNormalAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_normal = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Quad::SetTextureAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_texture = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

// draw Quad

void Quad::TransferData()
{
  glBindVertexArray(m_vao);

  if (m_buffers.find(m_vertex) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
    glBufferData(m_bufferTypes[m_vertex], 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);
  }

  if (m_buffers.find(m_normal) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_normal], m_buffers[m_normal]);
    glBufferData(m_bufferTypes[m_normal], 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);
  }

  if (m_buffers.find(m_texture) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_texture], m_buffers[m_texture]);
    glBufferData(m_bufferTypes[m_texture], 2 * m_size * sizeof(float), (void*)m_texcoord, GL_STATIC_DRAW);
  }
}

void Quad::ConnectBuffer()
{
  glBindVertexArray(m_vao);

  if (m_buffers.find(m_vertex) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
    glEnableVertexAttribArray(m_attributes[m_vertex]);
    glVertexAttribPointer(m_attributes[m_vertex], 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (m_buffers.find(m_normal) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_normal], m_buffers[m_normal]);
    glEnableVertexAttribArray(m_attributes[m_normal]);
    glVertexAttribPointer(m_attributes[m_normal], 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (m_buffers.find(m_texture) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_texture], m_buffers[m_texture]);
    glEnableVertexAttribArray(m_attributes[m_texture]);
    glVertexAttribPointer(m_attributes[m_texture], 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

void Quad::Draw(glm::mat4& inh_model)
{
  ConnectBuffer();
  glDrawArrays(GL_TRIANGLES, 0, m_size);
}
