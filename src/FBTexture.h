#ifndef FB_TEXTURE_H
#define FB_TEXTURE_H

#include <GL\glew.h>

class FBTexture
{
public:
  FBTexture(GLuint id);
  ~FBTexture();

  void Init(GLenum attachment, GLuint width, GLuint height);
  void InitDepth(GLuint width, GLuint height);
  void Bind();

  GLuint m_id;
};

#endif