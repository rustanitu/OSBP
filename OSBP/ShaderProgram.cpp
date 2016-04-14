#include "ShaderProgram.h"

#include <stdio.h>


ShaderProgram::ShaderProgram()
{
  m_id = glCreateProgram();
  if (m_id == 0)
    Error("Could not create program object");
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::CompileShader(GLuint id)
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

void ShaderProgram::LinkProgram()
{
  GLint status;
  glLinkProgram(m_id);
  glGetProgramiv(m_id, GL_LINK_STATUS, &status);
  if (!status)
  {
    GLint len;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
    char* message = (char*)malloc(len*sizeof(char));
    glGetProgramInfoLog(m_id, len, 0, message);
    Error(message);
    free(message);
  }
}

void ShaderProgram::CreateShader(const char* filepath, GLenum type)
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
  fclose(file);
}

void ShaderProgram::Error(const char* err)
{
  if (err)
    printf("%s\n", err);
}