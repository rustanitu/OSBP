#include "FrameBuffer.h"

#include <GL\freeglut.h>


FrameBuffer::FrameBuffer()
: m_depthtex(NULL)
{
}


FrameBuffer::~FrameBuffer()
{
  glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::Init(GLuint ntexbuffers, GLuint width, GLuint height, bool depthbuffer)
{
  m_ntexbuffers = ntexbuffers;
  m_depthbuffer = depthbuffer;

  glGenFramebuffers(1, &m_id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  GLuint* textures_id = new GLuint[ntexbuffers];
  GLuint depth_id;

  glGenTextures(ntexbuffers, textures_id);
  if (depthbuffer)
    glGenTextures(1, &depth_id);

  for (GLuint i = 0; i < ntexbuffers; ++i) {
    FBTexture* tex = new FBTexture(textures_id[i]);
    m_fbtextures.insert(std::make_pair(GL_COLOR_ATTACHMENT0 + i, tex));
  }

  if (m_depthbuffer) {
    m_depthtex = new FBTexture(depth_id);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  InitTextures(width, height);

  delete[] textures_id;
}

void FrameBuffer::InitTextures(GLuint width, GLuint height)
{
  for (GLuint i = 0; i < m_ntexbuffers; ++i) {
    m_fbtextures.at(GL_COLOR_ATTACHMENT0 + i)->Init(GL_COLOR_ATTACHMENT0 + i, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  if (m_depthbuffer)
  {
    m_depthtex->InitDepth(width, height);
  }

  GLenum* buffers = new GLenum[m_ntexbuffers];
  for (GLuint i = 0; i < m_ntexbuffers; ++i)
  {
    buffers[i] = GL_COLOR_ATTACHMENT0 + i;
  }

  glDrawBuffers(m_ntexbuffers, buffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  delete[] buffers;
}

void FrameBuffer::DrawBind()
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
}

void FrameBuffer::ReadBind()
{
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
}