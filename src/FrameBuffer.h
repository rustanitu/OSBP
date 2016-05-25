#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL\glew.h>
#include <map>

#include "FBTexture.h"

class FrameBuffer
{
public:
  FrameBuffer();
  ~FrameBuffer();

  void Init(GLuint ntexbuffers, GLuint width, GLuint height, bool depthbuffer = false);
  void InitTextures(GLuint width, GLuint height);
  void DrawBind();
  void ReadBind();

  FBTexture* GetTextureBuffer(GLenum attach)
  {
    return m_fbtextures.at(attach);
  }

  FBTexture* GetDepthBuffer(GLenum attach)
  {
    return m_depthtex;
  }

  GLuint m_id;
private:
  GLuint m_ntexbuffers;
  bool m_depthbuffer;
  std::map<GLenum, FBTexture*> m_fbtextures;
  FBTexture* m_depthtex;
};

#endif