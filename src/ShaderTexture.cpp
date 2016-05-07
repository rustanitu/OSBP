#include "ShaderTexture.h"

#include <GL\freeglut.h>


ShaderTexture::ShaderTexture() : m_bmp(NULL)
{
}


ShaderTexture::~ShaderTexture()
{
  glDeleteTextures(1, &m_id);
  imgDestroy(m_bmp);
}

void ShaderTexture::Init(char* file)
{
  m_bmp = imgReadBMP(file);
  glGenTextures(1, &m_id);
}

void ShaderTexture::LoadTexture()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0 + m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgGetWidth(m_bmp), imgGetHeight(m_bmp), 0, GL_RGB, GL_FLOAT, imgGetData(m_bmp));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
}