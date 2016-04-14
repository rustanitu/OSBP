#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderProgram.h"

ShaderProgram* shader = NULL;

// Initialization function
static void Init()
{
  // open GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
    exit(!1);
  }

  // init OpenGL state
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  shader = new ShaderProgram();
  shader->CreateShader("vshader.vert", GL_VERTEX_SHADER);
  shader->CreateShader("fshader.frag", GL_FRAGMENT_SHADER);
  glBindAttribLocation(shader->m_id, 0, "myVertex");
  glBindAttribLocation(shader->m_id, 1, "color");
  shader->LinkProgram();
}

// Reshape callback
static void Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
}

// Draw scene //
static void DrawScene()
{
  static float coord[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };

  static float color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };

  glUseProgram(shader->m_id);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, coord);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, color);
  glEnableVertexAttribArray(1);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Display callback
static void Display(void)
{
  int vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw scene
  DrawScene();
  glutSwapBuffers();
}

// Main function
int main(int argc, char* argv[])
{
  // open GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(800, 600);

  // create window
  glutCreateWindow("Triangle");
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);

  // initiate OpenGL context
  Init();

  // interact...
  glutMainLoop();
  return 0;
}