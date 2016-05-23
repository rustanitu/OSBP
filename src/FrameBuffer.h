#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL\glew.h>

class FrameBuffer
{
public:
  FrameBuffer();
  ~FrameBuffer();

  void Init();
  void LoadTexture();

  GLuint m_id;
private:
};

#endif