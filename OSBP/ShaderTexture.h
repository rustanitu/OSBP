#ifndef SHADER_TEXTURE_H
#define SHADER_TEXTURE_H

extern "C" {
#include <Image\image.h>
}
#include <GL\glew.h>

class ShaderTexture
{
public:
  ShaderTexture();
  ~ShaderTexture();

  void Init(char* file);
  void LoadTexture();

  GLuint m_id;
private:
  Image* m_bmp;
};

#endif