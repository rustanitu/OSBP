#include"Sphere.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <math.h>

#define PI 3.14159265359

struct VertexInfo
{
  glm::vec3 pos;
  glm::vec2 st;
  glm::vec3 tan;
  glm::vec3 bitan;
};

Sphere::Sphere(float r, int nslices, int nstacks)
{
  int i, j;
  float tx_theta, tx_phi;

  tx_theta = (2 * PI) / nslices;
  tx_phi = (PI) / nstacks;
  VertexInfo** grid = new VertexInfo*[nstacks + 1];
  for (i = 0; i <= nstacks; i++)
  {
    float phi = tx_phi*i;
    float sinphi = sin(phi);
    float cosphi = cos(phi);
    grid[i] = new VertexInfo[nslices + 1];
    for (j = 0; j <= nslices; j++)
    {
      float theta = (j < nslices) ? tx_theta*j : 0.0f;
      float sintheta = sin(theta);
      float costheta = cos(theta);

      float x = r*costheta*sinphi;
      float y = r*cosphi;
      float z = r*sintheta*sinphi;

      float s = j / (float)nslices;
      float t = i / (float)nstacks;
      grid[i][j].pos = glm::vec3(x, y, z);
      grid[i][j].st = glm::vec2(s, t);
    }
  }

  for (int i = 0; i <= nslices; i++)
  {
    int ni = (i + 1) % (nslices + 1);
    for (int j = 0; j <= nstacks; j++)
    {
      int nj = (j + 1) % (nstacks + 1);
      SetTangentVectors(grid[j][i], grid[j][ni], grid[nj][i]);
      SetTangentVectors(grid[nj][i], grid[j][ni], grid[nj][ni]);
    }
  }

  m_size = nslices * nstacks * 6;
  m_vertices = new float[m_size * 3];
  m_texcoord = new float[m_size * 2];
  m_tangents = new float[m_size * 3];
  m_bitangents = new float[m_size * 3];
  int v = 0, c = 0;
  for (i = 0; i < nslices; i++)
  {
    for (j = 0; j < nstacks; j++)
    {
      m_vertices[v] = grid[j][i].pos.x;
      m_tangents[v] = grid[j][i].tan.x;
      m_bitangents[v++] = grid[j][i].bitan.x;
      m_vertices[v] = grid[j][i].pos.y;
      m_tangents[v] = grid[j][i].tan.y;
      m_bitangents[v++] = grid[j][i].bitan.y;
      m_vertices[v] = grid[j][i].pos.z;
      m_tangents[v] = grid[j][i].tan.z;
      m_bitangents[v++] = grid[j][i].bitan.z;
      m_texcoord[c++] = grid[j][i].st.s;
      m_texcoord[c++] = grid[j][i].st.t;

      m_vertices[v] = grid[j][i + 1].pos.x;
      m_tangents[v] = grid[j][i + 1].tan.x;
      m_bitangents[v++] = grid[j][i + 1].bitan.x;
      m_vertices[v] = grid[j][i + 1].pos.y;
      m_tangents[v] = grid[j][i + 1].tan.y;
      m_bitangents[v++] = grid[j][i + 1].bitan.y;
      m_vertices[v] = grid[j][i + 1].pos.z;
      m_tangents[v] = grid[j][i + 1].tan.z;
      m_bitangents[v++] = grid[j][i + 1].bitan.z;
      m_texcoord[c++] = grid[j][i + 1].st.s;
      m_texcoord[c++] = grid[j][i + 1].st.t;

      m_vertices[v] = grid[j + 1][i].pos.x;
      m_tangents[v] = grid[j + 1][i].tan.x;
      m_bitangents[v++] = grid[j + 1][i].bitan.x;
      m_vertices[v] = grid[j + 1][i].pos.y;
      m_tangents[v] = grid[j + 1][i].tan.y;
      m_bitangents[v++] = grid[j + 1][i].bitan.y;
      m_vertices[v] = grid[j + 1][i].pos.z;
      m_tangents[v] = grid[j + 1][i].tan.z;
      m_bitangents[v++] = grid[j + 1][i].bitan.z;
      m_texcoord[c++] = grid[j + 1][i].st.s;
      m_texcoord[c++] = grid[j + 1][i].st.t;

      m_vertices[v] = grid[j + 1][i].pos.x;
      m_tangents[v] = grid[j + 1][i].tan.x;
      m_bitangents[v++] = grid[j + 1][i].bitan.x;
      m_vertices[v] = grid[j + 1][i].pos.y;
      m_tangents[v] = grid[j + 1][i].tan.y;
      m_bitangents[v++] = grid[j + 1][i].bitan.y;
      m_vertices[v] = grid[j + 1][i].pos.z;
      m_tangents[v] = grid[j + 1][i].tan.z;
      m_bitangents[v++] = grid[j + 1][i].bitan.z;
      m_texcoord[c++] = grid[j + 1][i].st.s;
      m_texcoord[c++] = grid[j + 1][i].st.t;

      m_vertices[v] = grid[j][i + 1].pos.x;
      m_tangents[v] = grid[j][i + 1].tan.x;
      m_bitangents[v++] = grid[j][i + 1].bitan.x;
      m_vertices[v] = grid[j][i + 1].pos.y;
      m_tangents[v] = grid[j][i + 1].tan.y;
      m_bitangents[v++] = grid[j][i + 1].bitan.y;
      m_vertices[v] = grid[j][i + 1].pos.z;
      m_tangents[v] = grid[j][i + 1].tan.z;
      m_bitangents[v++] = grid[j][i + 1].bitan.z;
      m_texcoord[c++] = grid[j][i + 1].st.s;
      m_texcoord[c++] = grid[j][i + 1].st.t;

      m_vertices[v] = grid[j + 1][i + 1].pos.x;
      m_tangents[v] = grid[j + 1][i + 1].tan.x;
      m_bitangents[v++] = grid[j + 1][i + 1].bitan.x;
      m_vertices[v] = grid[j + 1][i + 1].pos.y;
      m_tangents[v] = grid[j + 1][i + 1].tan.y;
      m_bitangents[v++] = grid[j + 1][i + 1].bitan.y;
      m_vertices[v] = grid[j + 1][i + 1].pos.z;
      m_tangents[v] = grid[j + 1][i + 1].tan.z;
      m_bitangents[v++] = grid[j + 1][i + 1].bitan.z;
      m_texcoord[c++] = grid[j + 1][i + 1].st.s;
      m_texcoord[c++] = grid[j + 1][i + 1].st.t;
    }
  }

  for (i = 0; i <= nstacks; i++)
    delete[] grid[i];

  delete[] grid;
}

Sphere::~Sphere()
{
  delete[] m_vertices;
  delete[] m_texcoord;
  delete[] m_tangents;
  delete[] m_bitangents;
}

void Sphere::SetTangentVectors(VertexInfo& v0, VertexInfo& v1, VertexInfo& v2)
{
  glm::vec3 xst0(v0.pos.x, v0.st.s, v0.st.t);
  glm::vec3 xst1(v1.pos.x, v1.st.s, v1.st.t);
  glm::vec3 xst2(v2.pos.x, v2.st.s, v2.st.t);
  glm::vec3 ABCD0 = glm::cross(xst1 - xst0, xst2 - xst0);

  glm::vec3 yst0(v0.pos.y, v0.st.s, v0.st.t);
  glm::vec3 yst1(v1.pos.y, v1.st.s, v1.st.t);
  glm::vec3 yst2(v2.pos.y, v2.st.s, v2.st.t);
  glm::vec3 ABCD1 = glm::cross(yst1 - yst0, yst2 - yst0);

  glm::vec3 zst0(v0.pos.z, v0.st.s, v0.st.t);
  glm::vec3 zst1(v1.pos.z, v1.st.s, v1.st.t);
  glm::vec3 zst2(v2.pos.z, v2.st.s, v2.st.t);
  glm::vec3 ABCD2 = glm::cross(zst1 - zst0, zst2 - zst0);

  glm::vec3 tan = glm::vec3(-ABCD0.y / ABCD0.x, -ABCD1.y / ABCD1.x, -ABCD2.y / ABCD2.x);
  glm::vec3 bitan = glm::vec3(-ABCD0.z / ABCD0.x, -ABCD1.z / ABCD1.x, -ABCD2.z / ABCD2.x);

  v0.tan += tan;
  v0.bitan += bitan;

  v1.tan += tan;
  v1.bitan += bitan;

  v2.tan += tan;
  v2.bitan += bitan;
}

void Sphere::SetVertexAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_vertex = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Sphere::SetNormalAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_normal = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Sphere::SetTextureAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_texture = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Sphere::SetTangentAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_tangent = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

void Sphere::SetBitangentAttribute(std::string attname, GLuint location, GLenum bufferType)
{
  m_bitangent = attname;
  ShaderObject::SetAttribute(attname, location, bufferType);
}

// draw sphere

void Sphere::TransferData()
{
  glBindVertexArray(m_vao);

  glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
  glBufferData(m_bufferTypes[m_vertex], 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes[m_normal], m_buffers[m_normal]);
  glBufferData(m_bufferTypes[m_normal], 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes[m_texture], m_buffers[m_texture]);
  glBufferData(m_bufferTypes[m_texture], 2 * m_size * sizeof(float), (void*)m_texcoord, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes[m_tangent], m_buffers[m_tangent]);
  glBufferData(m_bufferTypes[m_tangent], 3 * m_size * sizeof(float), (void*)m_tangents, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes[m_bitangent], m_buffers[m_bitangent]);
  glBufferData(m_bufferTypes[m_bitangent], 3 * m_size * sizeof(float), (void*)m_bitangents, GL_STATIC_DRAW);
}

void Sphere::ConnectBuffer()
{
  glBindVertexArray(m_vao);

  glBindBuffer(m_bufferTypes[m_vertex], m_buffers[m_vertex]);
  glEnableVertexAttribArray(m_attributes[m_vertex]);
  glVertexAttribPointer(m_attributes[m_vertex], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes[m_normal], m_buffers[m_normal]);
  glEnableVertexAttribArray(m_attributes[m_normal]);
  glVertexAttribPointer(m_attributes[m_normal], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes[m_texture], m_buffers[m_texture]);
  glEnableVertexAttribArray(m_attributes[m_texture]);
  glVertexAttribPointer(m_attributes[m_texture], 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes[m_tangent], m_buffers[m_tangent]);
  glEnableVertexAttribArray(m_attributes[m_tangent]);
  glVertexAttribPointer(m_attributes[m_tangent], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes[m_bitangent], m_buffers[m_bitangent]);
  glEnableVertexAttribArray(m_attributes[m_bitangent]);
  glVertexAttribPointer(m_attributes[m_bitangent], 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Sphere::Draw()
{
  ConnectBuffer();
  glDrawArrays(GL_TRIANGLES, 0, m_size);
}
