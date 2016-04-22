#include "ShaderProgram.h"

#include <stdio.h>
#include <string>


ShaderProgram::ShaderProgram()
  : m_vid(0), m_fid(0), m_bufsize(0)
{
  m_id = glCreateProgram();
  if (m_id == 0)
    Error("Could not create program object");
}

ShaderProgram::~ShaderProgram()
{
  delete m_buffers;
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

void ShaderProgram::CreateShader(const char** shader, GLenum type)
{
  GLuint id = glCreateShader(type);
  if (id == 0)
    Error("Could not create vertex shader object");

  glShaderSource(id, 1, shader, 0);
  CompileShader(id);
  glAttachShader(m_id, id);

  switch (type)
  {
    case GL_VERTEX_SHADER:
      m_vid = id;
      break;
    case GL_FRAGMENT_SHADER:
      m_fid = id;
      break;
  }
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