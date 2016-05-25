#include "FBTexture.h"

#include <GL\freeglut.h>


FBTexture::FBTexture(GLuint id)
: m_id(id)
{
}


FBTexture::~FBTexture()
{
  glDeleteTextures(1, &m_id);
}

void FBTexture::Bind()
{
  glActiveTexture(GL_TEXTURE0 + m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

void FBTexture::Init(GLenum attachment, GLuint width, GLuint height)
{
  Bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_id, 0);
}

void FBTexture::InitDepth(GLuint width, GLuint height)
{
  Bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_id, 0);
}