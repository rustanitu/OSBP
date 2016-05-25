#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Manipulator.h"
#include "ShaderTexture.h"
#include "Quad.h"

#include <glm\gtc\matrix_transform.hpp>

const int W = 800;
const int H = 600;
ShaderProgram* scene_shader = NULL;
ShaderProgram* quad_shader = NULL;
Camera* cam = NULL;
Sphere sphere;
Quad quad;
GLuint vao, vbo[1];
Manipulator* manip = NULL;
ShaderTexture tex;
ShaderTexture texnormals;
ShaderTexture wall;


// Initialization function
//static void Init()
//{
//  // open GLEW
//  GLenum err = glewInit();
//  if (GLEW_OK != err)
//  {
//    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
//    exit(!1);
//  }
//  glutInitContextVersion(4, 5);
//  glutInitContextProfile(GLUT_CORE_PROFILE);
//
//  // init OpenGL state
//  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//  glEnable(GL_DEPTH_TEST);
//
//  scene_shader = new ShaderProgram();
//  scene_shader->Init();
//  scene_shader->CreateShader("shaders\\vshader.vert.glsl", GL_VERTEX_SHADER);
//  scene_shader->CreateShader("shaders\\fshader.frag.glsl", GL_FRAGMENT_SHADER);
//
//  sphere.Init(scene_shader);
//  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
//  sphere.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
//  sphere.SetTextureAttribute("texcoord", 2, GL_ARRAY_BUFFER);
//  sphere.SetTangentAttribute("tan", 3, GL_ARRAY_BUFFER);
//  sphere.SetBitangentAttribute("bitan", 4, GL_ARRAY_BUFFER);
//  sphere.TransferData();
//
//  tex.Init("..\\textures\\moon.bmp");
//  texnormals.Init("..\\textures\\moonnorm.bmp");
//
//  scene_shader->LinkProgram();
//
//  cam = new Camera(W, H);
//  cam->SetEye(0, 0, 3);
//  cam->SetupCamera();
//
//  sphere.SetManipulatorCamera(cam);
//  Manipulator::SetCurrent(sphere.GetManipulator());
//}

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
  scene_shader->UseProgram();

  glm::mat4 model = sphere.GetModel();
  glm::mat4 mvp = cam->m_proj * cam->m_view * model;
  scene_shader->SetUniform("mvp", mvp);
  scene_shader->SetUniform("model", model);
  scene_shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));

  scene_shader->SetUniform("light", cam->GetEye() + glm::vec3(0, 0, 10));
  scene_shader->SetUniform("eye", cam->GetEye());

  glm::vec3 white = glm::vec3(1, 1, 1);
  scene_shader->SetUniform("amb", white * 0.1f);
  scene_shader->SetUniform("diff", white * 1.0f);
  scene_shader->SetUniform("spec", white * 0.25f);
  scene_shader->SetUniform("shi", 200.0f);

  texnormals.LoadTexture();
  scene_shader->SetUniform("normtexture", texnormals.m_id);
  tex.LoadTexture();
  scene_shader->SetUniform("difftexture", tex.m_id);

  sphere.Draw();
}

void Init()
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

  scene_shader = new ShaderProgram();
  scene_shader->Init();
  scene_shader->CreateShader("shaders\\vshader.vert.glsl", GL_VERTEX_SHADER);
  scene_shader->CreateShader("shaders\\fshader.frag.glsl", GL_FRAGMENT_SHADER);
  scene_shader->LinkProgram();

  quad_shader = new ShaderProgram();
  quad_shader->Init();
  quad_shader->CreateShader("shaders\\quad.vert.glsl", GL_VERTEX_SHADER);
  quad_shader->CreateShader("shaders\\quad.frag.glsl", GL_FRAGMENT_SHADER);
  quad_shader->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetEye(0, 0, 3);
  cam->SetupCamera();

  sphere.Init(scene_shader);
  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
  sphere.SetTextureAttribute("texcoord", 2, GL_ARRAY_BUFFER);
  sphere.SetTangentAttribute("tan", 3, GL_ARRAY_BUFFER);
  sphere.SetBitangentAttribute("bitan", 4, GL_ARRAY_BUFFER);
  sphere.TransferData();
  
  sphere.SetManipulatorCamera(cam);
  Manipulator::SetCurrent(sphere.GetManipulator());

  quad.Init(quad_shader);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.SetTextureAttribute("texcoord", 1, GL_ARRAY_BUFFER);
  quad.TransferData();

  tex.Init("..\\textures\\moon.bmp");
  texnormals.Init("..\\textures\\moonnorm.bmp");
  wall.Init("..\\textures\\BrokenWall.bmp");
}

void SceneRender()
{
  scene_shader->UseProgram();

  glm::mat4 model = sphere.GetModel();
  glm::mat4 mvp = cam->m_proj * cam->m_view * model;
  scene_shader->SetUniform("mvp", mvp);
  scene_shader->SetUniform("model", model);
  scene_shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));

  scene_shader->SetUniform("light", cam->GetEye() + glm::vec3(0, 0, 10));
  scene_shader->SetUniform("eye", cam->GetEye());

  glm::vec3 white = glm::vec3(1, 1, 1);
  scene_shader->SetUniform("amb", white * 0.1f);
  scene_shader->SetUniform("diff", white * 1.0f);
  scene_shader->SetUniform("spec", white * 0.25f);
  scene_shader->SetUniform("shi", 200.0f);

  texnormals.LoadTexture();
  scene_shader->SetUniform("normtexture", texnormals.m_id);
  tex.LoadTexture();
  scene_shader->SetUniform("difftexture", tex.m_id);

  sphere.Draw();
}

void TestDraw()
{
  //* FrameBuffer Init
  GLuint frameBuffer;
  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


  // Texture Init to receive the framebuffer render
  GLuint texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);


  // Depth Init
  GLuint rboDepthStencil;
  glGenRenderbuffers(1, &rboDepthStencil);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W, H);


  // Attach Depth
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);


  // Render to FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  //glEnable(GL_DEPTH_TEST);
  //*/

  //* Scene Render
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.2, 0.2, 0.6, 1);
  glEnable(GL_DEPTH_TEST);
  //SceneRender();
  quad_shader->UseProgram();
  wall.LoadTexture();
  quad_shader->SetUniform("tex", wall.m_id);
  quad.Draw();
  //*/


  //* Bind default framebuffer and draw contents of our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  //*/


  //* Quad Render with Texture
  quad_shader->UseProgram();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  quad_shader->SetUniform("tex", texColorBuffer);
  quad.Draw();
  //*/


  //* Depth delete
  glDeleteRenderbuffers(1, &rboDepthStencil);


  // Texture delete
  glDeleteTextures(1, &texColorBuffer);

  // FrameBuffer Delete
  glDeleteFramebuffers(1, &frameBuffer);
  //*/
}

// Display callback
static void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw scene
  //DrawScene();
  TestDraw();
  glutSwapBuffers();
  glutPostRedisplay();
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