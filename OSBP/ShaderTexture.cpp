#include "ShaderTexture.h"

#include <GL\glew.h>
#include <GL\freeglut.h>


ShaderTexture::ShaderTexture()
{
}


ShaderTexture::~ShaderTexture()
{
}

void ShaderTexture::Init(char* file)
{
  bmpfile = readBmpFile(file);
  glGenTextures(1, &m_id);
}

void ShaderTexture::LoadTexture()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(bmpfile), getHeight(bmpfile), 0, GL_BGR, GL_FLOAT, getData(bmpfile));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glGenerateMipmap(GL_TEXTURE_2D);
}