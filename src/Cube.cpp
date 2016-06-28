#include"Cube.h"

Cube::Cube(float l)
{
  float d = l / 2.0f;

  float vertices[] = { 
    -d, -d, +d, +d, -d, +d, -d, +d, +d, -d, +d, +d, +d, -d, +d, +d, +d, +d,
    -d, +d, -d, +d, +d, -d, -d, +d, +d, -d, +d, +d, +d, +d, -d, +d, +d, +d,
    +d, -d, -d, +d, +d, -d, +d, -d, +d, +d, -d, +d, +d, +d, -d, +d, +d, +d,
    +d, +d, -d, -d, +d, -d, +d, -d, -d, +d, -d, -d, -d, +d, -d, -d, -d, -d,
    +d, -d, +d, -d, -d, +d, +d, -d, -d, +d, -d, -d, -d, -d, +d, -d, -d, -d,
    -d, +d, +d, -d, -d, +d, -d, +d, -d, -d, +d, -d, -d, -d, +d, -d, -d, -d
  };

  m_size = 6 * 6;
  m_vertices = new float[m_size * 3];

  m_vertices = (float*)memcpy(m_vertices, vertices, m_size * 3 * sizeof(float));
}

Cube::~Cube()
{
  delete[] m_vertices;
}

void Cube::SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_vertex = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

// draw Cube

void Cube::TransferData()
{
  glBindVertexArray(m_vao);

  if (m_buffers.find(m_vertex) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
    glBufferData(m_bufferTypes[m_vertex], 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);
  }
}

void Cube::ConnectBuffer()
{
  glBindVertexArray(m_vao);

  if (m_buffers.find(m_vertex) != m_buffers.end())
  {
    glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
    glEnableVertexAttribArray(m_attributes[m_vertex]);
    glVertexAttribPointer(m_attributes[m_vertex], 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

void Cube::Draw(glm::mat4& inh_model)
{
  ConnectBuffer();
  glDrawArrays(GL_TRIANGLES, 0, m_size);
}
