#ifndef FB_TEXTURE_H
#define FB_TEXTURE_H

#include <GL\glew.h>

class FBTexture
{
public:
  FBTexture(GLuint width, GLuint height);
  ~FBTexture();

  void Init(GLint format, GLenum datatype, GLenum attachment);
  void LoadTexture();

  void SetDimensions(GLuint width, GLuint height)
  {
    m_width = width;
    m_height = height;
  }

  GLuint m_id;
private:
  GLuint m_height;
  GLuint m_width;
  GLint m_format;
  GLenum m_datatype;
  GLenum m_attachment;
};

#endif