#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <map>

class ShaderProgram
{
public:
  enum ATTRIBUTE_TYPE
  {
    TRIANGLE_VERTICES,
    PRIMITIVE_VERTICES,
    GENERIC
  };

  ShaderProgram();
  ~ShaderProgram();

  // Link Shader //
  void LinkProgram();

  // Cria programa de shader //
  void CreateShader(const char* filepath, GLenum type);
  void CreateShader(const char** shader, GLenum type);

  // Buffer Management
  GLuint CreateBuffer(GLsizei n)
  {
    GLuint* buffers = new GLuint[n];
    glGenBuffers(n, buffers);

    GLuint first;
    if (!m_buffers)
    {
      m_buffers = buffers;
      m_bufsize = n;
      first = buffers[0];
    }
    else
    {
      m_buffers = (GLuint*)realloc(m_buffers, m_bufsize + n);
      m_buffers = (GLuint*)memcpy(m_buffers + m_bufsize, buffers, n);
      m_bufsize = m_bufsize + n;
      first = buffers[0];
      delete buffers;
    }

    return first;
  }

  void DeleteBuffer(GLuint first, GLsizei n)
  {
    glDeleteBuffers(n, &m_buffers[first]);
  }

  void BindBuffer(GLuint id, GLenum type)
  {
    glBindBuffer(type, m_buffers[id]);
  }

  void ConectBuffer(GLenum type)
  {
    //switch (type)
    //{
    //  case GL_ARRAY_BUFFER:
    //    glVertexAttribPointer(id, n, GL_FLOAT, GL_FALSE, 0, 0, vertices);
    //}
  }

  void SetVao(const char* name, void * attrib)
  {
    GLuint id;
    glGenVertexArrays(1, &id);
    m_attributes.insert(std::make_pair(name, id));
  }

  void SetUniform(const char* name, void * uniform)
  {
    GLuint id;
    glGenBuffers(1, &id);
    m_attributes.insert(std::make_pair(name, id));
  }

  void SetAttrib(const char* name, void * attrib)
  {
    GLuint id;
    glGenBuffers(1, &id);
    m_attributes.insert(std::make_pair(name, id));
  }

private:
  void Error(const char* err);

  // Compile Shader
  void CompileShader(GLuint id);

public:
  GLuint m_id;
  GLuint m_vid;
  GLuint m_fid;

private:
  GLuint* m_buffers;
  GLsizei m_bufsize;
  std::map<const char*, GLuint> m_attributes;
};

#endif