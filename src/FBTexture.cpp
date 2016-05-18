#include "FBTexture.h"

#include <GL\freeglut.h>


FBTexture::FBTexture(GLuint width, GLuint height)
{
  SetDimensions(width, height);
}


FBTexture::~FBTexture()
{
  glDeleteTextures(1, &m_id);
}

void FBTexture::Init(GLint format, GLenum datatype, GLenum attachment)
{
  m_format = format;
  m_datatype = datatype;
  m_attachment = attachment;
  glGenTextures(1, &m_id);
}

void FBTexture::LoadTexture()
{
  glEnable(GL_TEXTURE_2D);
  // ???? glActiveTexture(GL_TEXTURE0 + m_id);     ???
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, m_datatype, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachment, GL_TEXTURE_2D, m_id, 0);
}