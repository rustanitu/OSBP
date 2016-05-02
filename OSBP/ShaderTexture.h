#ifndef SHADER_TEXTURE_H
#define SHADER_TEXTURE_H

#include <Image\BmpReader.h>
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
  BMPR::BmpFile bmpfile;
};

#endif