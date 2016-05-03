#include"Sphere.h"

#include <math.h>

#define PI 3.14159265359

Sphere::Sphere(float radius, int nslices, int nstacks)
{
  int i, j;
  float theta, phi, tx_theta, tx_phi;
  float x, y, z;

  tx_theta = (2 * PI) / nslices;
  tx_phi = (PI) / nstacks;
  float*** grid = new float**[nstacks + 1];
  for (i = 0; i <= nstacks; i++)
  {
    phi = tx_phi*i;
    grid[i] = new float*[nslices + 1];
    for (j = 0; j <= nslices; j++)
    {
      grid[i][j] = new float[5];
      theta = (j < nslices) ? tx_theta*j : 0.0f;
      x = radius*cos(theta)*sin(phi);
      y = radius*cos(phi);
      z = radius*sin(theta)*sin(phi);
      grid[i][j][0] = x;
      grid[i][j][1] = y;
      grid[i][j][2] = z;
      grid[i][j][3] = j / (float)nslices;
      grid[i][j][4] = i / (float)nstacks;
    }
  }

  m_size = nslices * nstacks * 6;
  m_vertices = new float[m_size * 3];
  m_texcoord = new float[m_size * 2];
  int v = 0, c = 0;
  for (i = 0; i < nslices; i++)
  {
    for (j = 0; j < nstacks; j++)
    {
      m_vertices[v++] = grid[j][i][0];
      m_vertices[v++] = grid[j][i][1];
      m_vertices[v++] = grid[j][i][2];
      m_texcoord[c++] = grid[j][i][3];
      m_texcoord[c++] = grid[j][i][4];

      m_vertices[v++] = grid[j][i + 1][0];
      m_vertices[v++] = grid[j][i + 1][1];
      m_vertices[v++] = grid[j][i + 1][2];
      m_texcoord[c++] = grid[j][i + 1][3];
      m_texcoord[c++] = grid[j][i + 1][4];

      m_vertices[v++] = grid[j + 1][i][0];
      m_vertices[v++] = grid[j + 1][i][1];
      m_vertices[v++] = grid[j + 1][i][2];
      m_texcoord[c++] = grid[j + 1][i][3];
      m_texcoord[c++] = grid[j + 1][i][4];

      m_vertices[v++] = grid[j + 1][i][0];
      m_vertices[v++] = grid[j + 1][i][1];
      m_vertices[v++] = grid[j + 1][i][2];
      m_texcoord[c++] = grid[j + 1][i][3];
      m_texcoord[c++] = grid[j + 1][i][4];

      m_vertices[v++] = grid[j][i + 1][0];
      m_vertices[v++] = grid[j][i + 1][1];
      m_vertices[v++] = grid[j][i + 1][2];
      m_texcoord[c++] = grid[j][i + 1][3];
      m_texcoord[c++] = grid[j][i + 1][4];

      m_vertices[v++] = grid[j + 1][i + 1][0];
      m_vertices[v++] = grid[j + 1][i + 1][1];
      m_vertices[v++] = grid[j + 1][i + 1][2];
      m_texcoord[c++] = grid[j + 1][i + 1][3];
      m_texcoord[c++] = grid[j + 1][i + 1][4];
    }
  }

  delete grid;
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
}

void Sphere::Draw()
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

  glDrawArrays(GL_TRIANGLES, 0, m_size);
}

void Sphere::DrawWire()
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

  glDrawArrays(GL_LINES, 0, m_size);
}