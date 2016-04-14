#include "Shader.h"

#include <stdio.h>


Shader::Shader()
{
  m_id = glCreateProgram();
  if (m_id == 0)
    Error("Could not create program object");
}

Shader::~Shader()
{
}

void Shader::CompileShader(GLuint id)
{
  GLint status;
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    GLint len;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
    char* message = (char*)malloc(len*sizeof(char));
    glGetShaderInfoLog(id, len, 0, message);
    Error(message);
    free(message);
  }
}

void Shader::LinkProgram(GLuint id)
{
  GLint status;
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (!status)
  {
    GLint len;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
    char* message = (char*)malloc(len*sizeof(char));
    glGetProgramInfoLog(id, len, 0, message);
    Error(message);
    free(message);
  }
}

void Shader::CreateShader(const char* filepath, GLenum type)
{
  GLuint id = glCreateShader(type);

  if (id == 0)
    Error("Could not create vertex shader object");

  FILE* file = new FILE();
  if (!file)
    return;

  fopen_s(&file, filepath, "r");
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char* strfile = new char[size];
  fread(strfile, sizeof(char), size, file);

  glShaderSource(id, 1, &strfile, 0);
  CompileShader(id);

  glAttachShader(m_id, id);
  LinkProgram(m_id);

  fclose(file);
}

void Shader::Error(const char* err)
{
  if (err)
    printf("%s\n", err);
}