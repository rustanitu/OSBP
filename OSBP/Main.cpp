#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Manipulator.h"
#include "ShaderTexture.h"

#include <glm\gtc\matrix_transform.hpp>

const int W = 800;
const int H = 600;
ShaderProgram* shader = NULL;
Camera* cam = NULL;
Sphere sphere;
GLuint vao, vbo[1];
VManipulator* manip = NULL;
ShaderTexture tex;
ShaderTexture texnormals;
glm::mat4 model;


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
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  // init OpenGL state
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  shader = new ShaderProgram();
  shader->Init();
  shader->CreateShader("vshader.vert.glsl", GL_VERTEX_SHADER);
  shader->CreateShader("fshader.frag.glsl", GL_FRAGMENT_SHADER);

  sphere.Init(shader);
  sphere.SetAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.SetAttribute("normal", 1, GL_ARRAY_BUFFER);
  sphere.SetAttribute("texcoord", 2, GL_ARRAY_BUFFER);
  sphere.SetAttribute("tan", 3, GL_ARRAY_BUFFER);
  sphere.SetAttribute("binorm", 4, GL_ARRAY_BUFFER);
  sphere.TransferData();

  tex.Init("textures\\moon.bmp");
  texnormals.Init("textures\\moonnorm.bmp");

  shader->LinkProgram();

  cam = new Camera(W, H);
  cam->SetEye(0, 0, 2.4);
  cam->SetupCamera();

  manip = new VManipulator(&model);
}

// Reshape callback
static void Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  cam->SetViewport(w, h);
  cam->SetupCamera();
}

// Draw scene //
static void DrawScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  shader->UseProgram();

  glm::mat4 mvp = cam->m_proj * cam->m_view * model;
  shader->SetUniform("mvp", mvp);
  shader->SetUniform("model", model);
  shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));

  shader->SetUniform("light", glm::vec3(-5, 0, 0));
  shader->SetUniform("eye", cam->GetEye());

  glm::vec3 white = glm::vec3(1, 1, 1);
  shader->SetUniform("amb", white * 0.1f);
  shader->SetUniform("diff", white * 1.0f);
  shader->SetUniform("spec", white * 0.25f);
  shader->SetUniform("shi", 10.0f);

  shader->SetUniform("normtexture", texnormals.m_id);
  shader->SetUniform("difftexture", tex.m_id);

  texnormals.LoadTexture();
  tex.LoadTexture();
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
  glutCreateWindow("Bump Mapping Sphere");
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);

  // initiate OpenGL context
  Init();

  // interact...
  glutMainLoop();
  return 0;
}