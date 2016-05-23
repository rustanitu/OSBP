#include "FrameBuffer.h"

#include <GL\freeglut.h>


FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
  glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::Init()
{
  glGenFramebuffers(1, &m_id);
}

void FrameBuffer::LoadTexture()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, DrawBuffers);
}