#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <GL\freeglut.h>

class Shader
{
public:
  Shader();
  ~Shader();

  void CompileShader(GLuint id);

  // Link Shader //
  void LinkProgram(GLuint id);

  // Cria programa de shader //
  void CreateShader(const char* filepath, GLenum type);

private:
  void Error(const char* err);

public:
  GLuint m_id;

private:
  GLuint* m_id_shaders;
  const char** m_shaders;
};

#endif