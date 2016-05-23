#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Manipulator.h"
#include "ShaderTexture.h"
#include "FBTexture.h"
#include "Quad.h"
#include "FrameBuffer.h"
#include <glm\gtc\matrix_transform.hpp>

const int W = 800;
const int H = 600;
ShaderProgram* shader = NULL;
ShaderProgram* shader2 = NULL;
Camera* cam = NULL;
Sphere sphere;
Manipulator* manip = NULL;
ShaderTexture tex;
ShaderTexture texnormals;

//FBTexture verttex;
//FBTexture normtex;
//FBTexture ambtex;
//FBTexture difftex;
//FBTexture spectex;

FBTexture gbuffer(W, H);

Quad quad;

FrameBuffer frameBuffer;



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

  cam = new Camera(W, H);
  cam->SetEye(0, 0, 3);
  cam->SetupCamera();

  shader = new ShaderProgram();
  shader->Init();
  shader->CreateShader("shaders\\bumpmap.vert.glsl", GL_VERTEX_SHADER);
  shader->CreateShader("shaders\\bumpmap.frag.glsl", GL_FRAGMENT_SHADER);
  shader->LinkProgram();

  shader2 = new ShaderProgram();
  shader2->Init();
  //shader2->CreateShader("shaders\\deferred.vert.glsl", GL_VERTEX_SHADER);
  shader2->CreateShader("shaders\\deferred.frag.glsl", GL_FRAGMENT_SHADER);
  shader2->LinkProgram();

  sphere.Init();
  sphere.SetShader(shader);
  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
  sphere.SetTextureAttribute("texcoord", 2, GL_ARRAY_BUFFER);
  sphere.SetTangentAttribute("tan", 3, GL_ARRAY_BUFFER);
  sphere.SetBitangentAttribute("bitan", 4, GL_ARRAY_BUFFER);
  sphere.TransferData();
  sphere.SetManipulatorCamera(cam);
  Manipulator::SetCurrent(sphere.GetManipulator());

  quad.Init();
  quad.SetShader(shader2);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.SetTextureAttribute("st", 1, GL_ARRAY_BUFFER);
  quad.TransferData();
  quad.SetManipulatorCamera(cam);
  //Manipulator::SetCurrent(quad.GetManipulator());

  tex.Init("..\\textures\\moon.bmp");
  texnormals.Init("..\\textures\\moonnorm.bmp");

  //verttex.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);
  //normtex.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1);
  //ambtex.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT2);
  //difftex.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT3);
  //spectex.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT4);
  
  frameBuffer.Init();
  frameBuffer.LoadTexture();

  gbuffer.Init(GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0);
  gbuffer.LoadTexture();
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
  frameBuffer.LoadTexture();
  gbuffer.LoadTexture();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->UseProgram();

  glm::mat4 model = sphere.GetModel();
  glm::mat4 mvp = cam->m_proj * cam->m_view * model;
  shader->SetUniform("mvp", mvp);
  shader->SetUniform("model", model);
  shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));

  shader->SetUniform("light", cam->GetEye() + glm::vec3(-999999999, 0, 0));
  shader->SetUniform("eye", cam->GetEye());

  glm::vec3 white = glm::vec3(1, 1, 1);
  shader->SetUniform("amb", white * 0.1f);
  shader->SetUniform("diff", white * 1.0f);
  shader->SetUniform("spec", white * 0.15f);
  shader->SetUniform("shi", 120.0f);

  shader->SetUniform("normtexture", texnormals.m_id);
  shader->SetUniform("difftexture", tex.m_id);

  texnormals.LoadTexture();
  tex.LoadTexture();
  
  sphere.Draw();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader2->UseProgram();

  shader2->SetUniform("tex", gbuffer.m_id);
  quad.Draw();
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