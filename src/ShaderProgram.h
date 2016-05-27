#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

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
  void Init();
  ~ShaderProgram();

  // Link Shader //
  void LinkProgram();

  // Cria programa de shader //
  void CreateShader(const char* filepath, GLenum type);
  void CreateShader(const char** shader, GLenum type);

  GLuint GetId()
  {
    return m_id;
  }

  void UseProgram()
  {
    glUseProgram(m_id);
  }

  void SetUniform(const char* name, GLuint uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniform1i(id, uniform);
  }

  void SetUniform(const char* name, int uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniform1i(id, uniform);
  }

  void SetUniform(const char* name, float uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniform1f(id, uniform);
  }

  void SetUniform(const char* name, glm::vec3 uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniform3fv(id, 1, &uniform[0]);
  }

  void SetUniform(const char* name, GLuint n, glm::vec3* uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    float* cores = new float[n*3];
    int j = 0;
    for (int i = 0; i < n; i++)
    {
      cores[j++] = uniform[i].x;
      cores[j++] = uniform[i].y;
      cores[j++] = uniform[i].z;
    }
    glUniform3fv(id, n, cores);
  }

  void SetUniform(const char* name, glm::vec4 uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniform4fv(id, 1, &uniform[0]);
  }

  void SetUniform(const char* name, glm::mat4 uniform)
  {
    GLuint id = glGetUniformLocation(m_id, name);
    glUniformMatrix4fv(id, 1, GL_FALSE, &uniform[0][0]);
  }

private:
  void Error(const char* err);

  // Compile Shader
  void CompileShader(GLuint id);

public:
  GLuint m_id;
};

#endif