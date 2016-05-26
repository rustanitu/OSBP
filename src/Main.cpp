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
ShaderProgram* blue_sphere_shader = NULL;
Camera* cam = NULL;
Sphere sphere;
Sphere blue_sphere;
Quad quad;
GLuint vao, vbo[1];
Manipulator* manip = NULL;
ShaderTexture tex;
ShaderTexture texnormals;
ShaderTexture wall;
GLuint _fb, _fb_tex_p, _fb_tex_n, _rb;

// Reshape callback
static void Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  cam->SetViewport(w, h);
  cam->SetupCamera();
}

void FramebufferInit()
{
  //* FrameBuffer Init
  glGenFramebuffers(1, &_fb);
  glBindFramebuffer(GL_FRAMEBUFFER, _fb);


  // Texture Init to receive the framebuffer render
  glGenTextures(1, &_fb_tex_p);
  glBindTexture(GL_TEXTURE_2D, _fb_tex_p);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fb_tex_p, 0);


  // Depth Init
  glGenRenderbuffers(1, &_rb);
  glBindRenderbuffer(GL_RENDERBUFFER, _rb);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W, H);


  // Attach Depth
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rb);
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

  blue_sphere_shader = new ShaderProgram();
  blue_sphere_shader->Init();
  blue_sphere_shader->CreateShader("shaders\\world.vert.glsl", GL_VERTEX_SHADER);
  blue_sphere_shader->CreateShader("shaders\\color.frag.glsl", GL_FRAGMENT_SHADER);
  blue_sphere_shader->LinkProgram();
  
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

  blue_sphere.Init(scene_shader);
  blue_sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  blue_sphere.TransferData();
  
  sphere.SetManipulatorCamera(cam);
  Manipulator::SetCurrent(sphere.GetManipulator());

  quad.Init(quad_shader);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.SetTextureAttribute("texcoord", 1, GL_ARRAY_BUFFER);
  quad.TransferData();

  tex.Init("..\\textures\\moon.bmp");
  texnormals.Init("..\\textures\\moonnorm.bmp");
  wall.Init("..\\textures\\BrokenWall.bmp");

  FramebufferInit();
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

  glm::vec3 white(1, 1, 1);
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

void BlueSphereRender()
{
  blue_sphere_shader->UseProgram();
  blue_sphere_shader->SetUniform("mvp", cam->m_proj * cam->m_view);
  blue_sphere.Draw();
}

void QuadRender()
{
  quad_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + _fb_tex_p);
  glBindTexture(GL_TEXTURE_2D, _fb_tex_p);
  quad_shader->SetUniform("tex", _fb_tex_p);

  glActiveTexture(GL_TEXTURE0 + _fb_tex_n);
  glBindTexture(GL_TEXTURE_2D, _fb_tex_n);
  quad_shader->SetUniform("tex2", _fb_tex_n);

  quad.Draw();
}

void DrawScene()
{
  glBindFramebuffer(GL_FRAMEBUFFER, _fb);
  GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, draw_bufs);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.2, 0.2, 0.6, 1);
  glEnable(GL_DEPTH_TEST);


  //* Scene Render
  SceneRender();
  //*/

  // deferred pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.52, 0.52, 0.2, 1.0f); // added ambient light here
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  //* Quad Render with Texture
  QuadRender();
  //*/
}

// Display callback
static void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw scene
  DrawScene();
  
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