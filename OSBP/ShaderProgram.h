#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL\glew.h>
#include <GL\freeglut.h>

class ShaderProgram
{
public:
  ShaderProgram();
  ~ShaderProgram();

  // Link Shader //
  void LinkProgram();

  // Cria programa de shader //
  void CreateShader(const char* filepath, GLenum type);

  // Bind
  void BindAttribute() {}

private:
  void Error(const char* err);

  // Compile Shader
  void CompileShader(GLuint id);

public:
  GLuint m_id;

private:
  GLuint* m_id_shaders;
  const char** m_shaders;
};

#endif