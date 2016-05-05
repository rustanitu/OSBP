#include"Sphere.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <math.h>

#define PI 3.14159265359

struct VertexInfo
{
  glm::vec3 pos;
  glm::vec2 st;
  glm::vec3 tan;
  glm::vec3 binorm;
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
    float phi = tx_phi*i; // => Pi*t
    float sinphi = sin(phi);
    float cosphi = cos(phi);
    grid[i] = new VertexInfo[nslices + 1];
    for (j = 0; j <= nslices; j++)
    {
      float theta = (j < nslices) ? tx_theta*j : 0.0f; // => 2*PI*s
      float sintheta = sin(theta);
      float costheta = cos(theta);

      float x = r*costheta*sinphi;
      float y = r*cosphi;
      float z = r*sintheta*sinphi;

      float dxs = - 2*PI*PI*r*sintheta*sinphi;
      float dys = 0.0f;
      float dzs = 2*PI*PI*r*costheta*sinphi;

      float dxt = PI*PI*r*costheta*cosphi;
      float dyt = -PI*PI*r*sinphi;
      float dzt = PI*PI*r*sintheta*cosphi;

      float s = j / (float)nslices;
      float t = i / (float)nstacks;
      grid[i][j].pos = glm::vec3(x, y, z);
      grid[i][j].st = glm::vec2(s, t);
      grid[i][j].tan = glm::vec3(dxs, dys, dzs);
      grid[i][j].binorm = glm::vec3(dxt, dyt, dzt);
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
      m_bitangents[v++] = grid[j][i].binorm.x;
      m_vertices[v] = grid[j][i].pos.y;
      m_tangents[v] = grid[j][i].tan.y;
      m_bitangents[v++] = grid[j][i].binorm.y;
      m_vertices[v] = grid[j][i].pos.z;
      m_tangents[v] = grid[j][i].tan.z;
      m_bitangents[v++] = grid[j][i].binorm.z;
      m_texcoord[c++] = grid[j][i].st.s;
      m_texcoord[c++] = grid[j][i].st.t;

      m_vertices[v] = grid[j][i + 1].pos.x;
      m_tangents[v] = grid[j][i + 1].tan.x;
      m_bitangents[v++] = grid[j][i + 1].binorm.x;
      m_vertices[v] = grid[j][i + 1].pos.y;
      m_tangents[v] = grid[j][i + 1].tan.y;
      m_bitangents[v++] = grid[j][i + 1].binorm.y;
      m_vertices[v] = grid[j][i + 1].pos.z;
      m_tangents[v] = grid[j][i + 1].tan.z;
      m_bitangents[v++] = grid[j][i + 1].binorm.z;
      m_texcoord[c++] = grid[j][i + 1].st.s;
      m_texcoord[c++] = grid[j][i + 1].st.t;

      m_vertices[v] = grid[j + 1][i].pos.x;
      m_tangents[v] = grid[j + 1][i].tan.x;
      m_bitangents[v++] = grid[j + 1][i].binorm.x;
      m_vertices[v] = grid[j + 1][i].pos.y;
      m_tangents[v] = grid[j + 1][i].tan.y;
      m_bitangents[v++] = grid[j + 1][i].binorm.y;
      m_vertices[v] = grid[j + 1][i].pos.z;
      m_tangents[v] = grid[j + 1][i].tan.z;
      m_bitangents[v++] = grid[j + 1][i].binorm.z;
      m_texcoord[c++] = grid[j + 1][i].st.s;
      m_texcoord[c++] = grid[j + 1][i].st.t;

      m_vertices[v] = grid[j + 1][i].pos.x;
      m_tangents[v] = grid[j + 1][i].tan.x;
      m_bitangents[v++] = grid[j + 1][i].binorm.x;
      m_vertices[v] = grid[j + 1][i].pos.y;
      m_tangents[v] = grid[j + 1][i].tan.y;
      m_bitangents[v++] = grid[j + 1][i].binorm.y;
      m_vertices[v] = grid[j + 1][i].pos.z;
      m_tangents[v] = grid[j + 1][i].tan.z;
      m_bitangents[v++] = grid[j + 1][i].binorm.z;
      m_texcoord[c++] = grid[j + 1][i].st.s;
      m_texcoord[c++] = grid[j + 1][i].st.t;

      m_vertices[v] = grid[j][i + 1].pos.x;
      m_tangents[v] = grid[j][i + 1].tan.x;
      m_bitangents[v++] = grid[j][i + 1].binorm.x;
      m_vertices[v] = grid[j][i + 1].pos.y;
      m_tangents[v] = grid[j][i + 1].tan.y;
      m_bitangents[v++] = grid[j][i + 1].binorm.y;
      m_vertices[v] = grid[j][i + 1].pos.z;
      m_tangents[v] = grid[j][i + 1].tan.z;
      m_bitangents[v++] = grid[j][i + 1].binorm.z;
      m_texcoord[c++] = grid[j][i + 1].st.s;
      m_texcoord[c++] = grid[j][i + 1].st.t;

      m_vertices[v] = grid[j + 1][i + 1].pos.x;
      m_tangents[v] = grid[j + 1][i + 1].tan.x;
      m_bitangents[v++] = grid[j + 1][i + 1].binorm.x;
      m_vertices[v] = grid[j + 1][i + 1].pos.y;
      m_tangents[v] = grid[j + 1][i + 1].tan.y;
      m_bitangents[v++] = grid[j + 1][i + 1].binorm.y;
      m_vertices[v] = grid[j + 1][i + 1].pos.z;
      m_tangents[v] = grid[j + 1][i + 1].tan.z;
      m_bitangents[v++] = grid[j + 1][i + 1].binorm.z;
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
}

// draw sphere

void Sphere::TransferData()
{
  glBindVertexArray(m_vao);

  glBindBuffer(m_bufferTypes["vertex"], m_buffers["vertex"]);
  glBufferData(GL_ARRAY_BUFFER, 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes["normal"], m_buffers["normal"]);
  glBufferData(GL_ARRAY_BUFFER, 3 * m_size * sizeof(float), (void*)m_vertices, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes["texcoord"], m_buffers["texcoord"]);
  glBufferData(GL_ARRAY_BUFFER, 2 * m_size * sizeof(float), (void*)m_texcoord, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes["tan"], m_buffers["tan"]);
  glBufferData(GL_ARRAY_BUFFER, 3 * m_size * sizeof(float), (void*)m_tangents, GL_STATIC_DRAW);

  glBindBuffer(m_bufferTypes["binorm"], m_buffers["binorm"]);
  glBufferData(GL_ARRAY_BUFFER, 3 * m_size * sizeof(float), (void*)m_bitangents, GL_STATIC_DRAW);
}

void Sphere::ConnectBuffer()
{
  glBindVertexArray(m_vao);

  glBindBuffer(m_bufferTypes["vertex"], m_buffers["vertex"]);
  glEnableVertexAttribArray(m_attributes["vertex"]);
  glVertexAttribPointer(m_attributes["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes["normal"], m_buffers["normal"]);
  glEnableVertexAttribArray(m_attributes["normal"]);
  glVertexAttribPointer(m_attributes["normal"], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes["texcoord"], m_buffers["texcoord"]);
  glEnableVertexAttribArray(m_attributes["texcoord"]);
  glVertexAttribPointer(m_attributes["texcoord"], 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes["tan"], m_buffers["tan"]);
  glEnableVertexAttribArray(m_attributes["tan"]);
  glVertexAttribPointer(m_attributes["tan"], 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(m_bufferTypes["binorm"], m_buffers["binorm"]);
  glEnableVertexAttribArray(m_attributes["binorm"]);
  glVertexAttribPointer(m_attributes["binorm"], 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Sphere::Draw()
{
  ConnectBuffer();
  glDrawArrays(GL_TRIANGLES, 0, m_size);
}
