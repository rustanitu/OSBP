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

int W = 800;
int H = 600;
ShaderProgram* scene_shader = NULL;
ShaderProgram* quad_shader = NULL;
ShaderProgram* blue_sphere_shader = NULL;
Camera* cam = NULL;
GLuint n_spheres = 5;
Sphere sphere;
Sphere blue_sphere;
Quad quad;
GLuint vao, vbo[1];
Manipulator* manip = NULL;
ShaderTexture tex;
ShaderTexture texnormals;
ShaderTexture wall;

GLuint framebuffer;
GLuint vertex_buff;
GLuint normal_buff;
GLuint amb_buff;
GLuint diff_buff;
GLuint spec_buff;
GLuint depthbuffer;

ShaderProgram* first_pass = NULL;
ShaderProgram* second_pass = NULL;

void FramebufferInit()
{
  //* FrameBuffer Init
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


  // Vertex Texture Init
  glGenTextures(1, &vertex_buff);
  glBindTexture(GL_TEXTURE_2D, vertex_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, W, H, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vertex_buff, 0);


  // Normal Texture Init
  glGenTextures(1, &normal_buff);
  glBindTexture(GL_TEXTURE_2D, normal_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, W, H, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_buff, 0);


  // Ambient Texture Init
  glGenTextures(1, &amb_buff);
  glBindTexture(GL_TEXTURE_2D, amb_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, W, H, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, amb_buff, 0);


  // Diffuse Texture Init
  glGenTextures(1, &diff_buff);
  glBindTexture(GL_TEXTURE_2D, diff_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, W, H, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, diff_buff, 0);


  // Specular Texture Init
  glGenTextures(1, &spec_buff);
  glBindTexture(GL_TEXTURE_2D, spec_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, W, H, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, spec_buff, 0);


  // Depth Init
  glGenRenderbuffers(1, &depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W, H);


  // Attach Depth
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
}

void FramebufferFinish()
{
  //* FrameBuffer Finish
  glDeleteFramebuffers(1, &framebuffer);
  glDeleteTextures(1, &vertex_buff);
  glDeleteRenderbuffers(1, &depthbuffer);
}

// Reshape callback
static void Reshape(int w, int h)
{
  W = w;
  H = h;
  glViewport(0, 0, w, h);
  cam->SetViewport(w, h);
  cam->SetupCamera();
  FramebufferFinish();
  FramebufferInit();
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

  first_pass = new ShaderProgram();
  first_pass->Init();
  first_pass->CreateShader("shaders\\repass_d.vert.glsl", GL_VERTEX_SHADER);
  first_pass->CreateShader("shaders\\repass_d.frag.glsl", GL_FRAGMENT_SHADER);
  first_pass->LinkProgram();

  second_pass = new ShaderProgram();
  second_pass->Init();
  second_pass->CreateShader("shaders\\lighting_d.vert.glsl", GL_VERTEX_SHADER);
  second_pass->CreateShader("shaders\\lighting_d.frag.glsl", GL_FRAGMENT_SHADER);
  second_pass->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetEye(0, 5, 8);
  cam->SetAt(0, 0, 2);
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

  blue_sphere.Init(first_pass);
  blue_sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  blue_sphere.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
  blue_sphere.TransferData();
  
  quad.Init(second_pass);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.SetTextureAttribute("texcoord", 1, GL_ARRAY_BUFFER);
  quad.TransferData();

  tex.Init("..\\textures\\moon.bmp");
  texnormals.Init("..\\textures\\moonnorm.bmp");
  wall.Init("..\\textures\\BrokenWall.bmp");

  FramebufferInit();
}

void MoonRender(Sphere& sphere, glm::mat4 t)
{
  scene_shader->UseProgram();

  glm::mat4 model = sphere.GetModel();
  glm::mat4 mvp = cam->m_proj * cam->m_view * t * model;
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

void BlueSphereRender(glm::mat4 t)
{
  first_pass->UseProgram();
  glm::mat4 mvp = cam->m_proj * cam->m_view * t;
  first_pass->SetUniform("mvp", mvp);
  first_pass->SetUniform("m", t);
  first_pass->SetUniform("_m", glm::transpose(glm::inverse(t)));

  glm::vec3 red(1, 0, 0);
  first_pass->SetUniform("mamb", red * 0.25f);
  first_pass->SetUniform("mdiff", red * 0.5f);
  first_pass->SetUniform("mspec", glm::vec3(1, 1, 1));
  first_pass->SetUniform("mshi", 64.0f);
  blue_sphere.Draw();
}

void SceneRender()
{
  int l = n_spheres;
  int n = l * l;
  for (int i = 0; i < n; i++)
  {
    float d = 2.5f;
    float z = ((i / l) - l / 2) * d;
    float x = ((i % l) - l / 2) * d;
    glm::mat4 t;
    t = glm::translate(t, glm::vec3(x, 0, z));
    //MoonRender(sphere, t);
    BlueSphereRender(t);
  }
}

void QuadRender()
{
  second_pass->UseProgram();

  glActiveTexture(GL_TEXTURE0 + vertex_buff);
  glBindTexture(GL_TEXTURE_2D, vertex_buff);
  second_pass->SetUniform("vertex_tex", vertex_buff);

  glActiveTexture(GL_TEXTURE0 + normal_buff);
  glBindTexture(GL_TEXTURE_2D, normal_buff);
  second_pass->SetUniform("normal_tex", normal_buff);

  glActiveTexture(GL_TEXTURE0 + amb_buff);
  glBindTexture(GL_TEXTURE_2D, amb_buff);
  second_pass->SetUniform("amb_tex", amb_buff);

  glActiveTexture(GL_TEXTURE0 + diff_buff);
  glBindTexture(GL_TEXTURE_2D, diff_buff);
  second_pass->SetUniform("diff_tex", diff_buff);

  glActiveTexture(GL_TEXTURE0 + spec_buff);
  glBindTexture(GL_TEXTURE_2D, spec_buff);
  second_pass->SetUniform("spec_tex", spec_buff);

  glm::vec3 red(0.4, 0.1, 0.1);
  second_pass->SetUniform("wrl_eye", cam->GetEye());
  second_pass->SetUniform("gamb", red * 0.2f);

  GLuint nl = 1;
  glm::vec3 light(0, 5, 0);
  second_pass->SetUniform("nlight", nl);
  second_pass->SetUniform("lpos", light);

  glm::vec3 white(1, 1, 1);
  second_pass->SetUniform("lamb",  white);
  second_pass->SetUniform("ldiff", white);
  second_pass->SetUniform("lspec", white);

  quad.Draw();
}

void DrawScene()
{
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
  glDrawBuffers(5, draw_bufs);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);


  //* Scene Render
  SceneRender();
  //*/

  // deferred pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 1);
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