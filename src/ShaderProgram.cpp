#include "ShaderProgram.h"

#include <stdio.h>
#include <string>


ShaderProgram::ShaderProgram()
: m_nshaders(0)
{
}

void ShaderProgram::Init()
{
  m_id = glCreateProgram();
  if (m_id == 0)
    Error("Could not create program object");
}

ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::CompileShader(GLuint id)
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
    return false;
  }

  m_shaders[m_nshaders] = id;
  m_nshaders++;
  return true;
}

bool ShaderProgram::LinkProgram()
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
    return false;
  }

  for (int i = 0; i < m_nshaders; ++i)
    glDetachShader(m_id, i);

  return true;
}

void ShaderProgram::CreateShader(const char** shader, GLenum type)
{
  GLuint id = glCreateShader(type);
  if (id == 0)
    Error("Could not create vertex shader object");

  glShaderSource(id, 1, shader, 0);
  if (CompileShader(id))
    glAttachShader(m_id, id);
}

void ShaderProgram::CreateShader(const char* filepath, GLenum type)
{
  FILE* file = new FILE();
  if (!file)
    return;

  fopen_s(&file, filepath, "rb");
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char* strfile = new char[size + 1];
  fread(strfile, sizeof(char), size, file);
  strfile[size] = '\0';
  
  CreateShader((const char**)&strfile, type);
  delete strfile;

  fclose(file);
}

void ShaderProgram::Error(const char* err)
{
  if (err)
    printf("%s\n", err);
}