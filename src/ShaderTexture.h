#ifndef SHADER_TEXTURE_H
#define SHADER_TEXTURE_H

extern "C" {
#include "image\Image.h"
}
#include <GL\glew.h>

class ShaderTexture
{
public:
  ShaderTexture();
  ~ShaderTexture();

  void Init(char* file);
  void LoadTexture();
  void Bind();

  GLuint m_id;
private:
  Image* m_bmp;
};

#endif