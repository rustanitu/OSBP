#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Manipulator.h"

#include <glm\gtc\matrix_transform.hpp>

const int W = 800;
const int H = 600;
ShaderProgram* shader = NULL;
Camera* cam = NULL;
Triangle triangle;
Sphere sphere;
GLuint vao;
VManipulator* manip = NULL;


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

  cam = new Camera(W, H);
  cam->SetEye(0, 0, 2);
  cam->SetupCamera();

  manip = new VManipulator(&cam->m_view);

  //triangle.SetVerticesAttribute(0);
  //triangle.SetColorsAttribute(1);
  sphere.SetVerticesAttribute(0);
  sphere.SetColorsAttribute(1);
}

// Reshape callback5
static void Reshape(int w, int h)
{
  //cam->SetViewport(w, h);
  //cam->SetupCamera();
}

// Draw scene //
static void DrawScene()
{
  glUseProgram(shader->m_id);
  glm::mat4 mvp = cam->m_proj * cam->m_view;
  GLuint MatrixID = glGetUniformLocation(shader->m_id, "mvp");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
  //triangle.Draw();
  sphere.Draw();
}

// Display callback
static void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw scene
  DrawScene();
  glutSwapBuffers();
}

// Main function
int main (int argc, char* argv[])
{
  // open GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(W, H);

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