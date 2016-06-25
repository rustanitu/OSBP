#include <stdio.h>
#include <time.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Manipulator.h"
#include "ShaderTexture.h"
#include "Quad.h"
#include "Group.h"

#include <glm\gtc\matrix_transform.hpp>

#define PI 3.14159265359
#define NSPHERE 5
#define NLIGHTS 20

int W = 800;
int H = 600;
Camera* cam = NULL;
Sphere blue_sphere;
Quad quad;

GLuint framebuffer;
GLuint vertex_buff;
GLuint normal_buff;
GLuint amb_buff;
GLuint diff_buff;
GLuint spec_buff;
GLuint depthbuffer;

ShaderProgram* first_pass = NULL;
ShaderProgram* second_pass = NULL;

const glm::vec3 WHITE(1.0f, 1.0f, 1.0f);
const glm::vec3 RED(1.0f, 0.0f, 0.0f);
const glm::vec3 GREEN(0.0f, 1.0f, 0.0f);
const glm::vec3 BLUE(0.0f, 0.0f, 1.0f);
const glm::vec3 YELLOW(1.0f, 1.0f, 0.0f);
const glm::vec3 PINK(1.0f, 0.0f, 1.0f);
const glm::vec3 CYAN(0.0f, 1.0f, 1.0f);
const glm::vec3 COLORS[7] = 
{
  WHITE,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN
};

glm::vec3 LIGHT_POS[NLIGHTS];
glm::vec3 LIGHT_SPOT[NLIGHTS];
glm::vec3 LIGHT_COLORS[NLIGHTS];
GLfloat THETA = 0;

ShaderProgram* shader = NULL;
Group* group = NULL;
Sphere moon;
Sphere moo;
ShaderTexture texmoon;
ShaderTexture texmoon_norm;

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
  //FramebufferFinish();
  //FramebufferInit();
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

  shader = new ShaderProgram();
  shader->Init();
  shader->CreateShader("shaders\\vshader.vert.glsl", GL_VERTEX_SHADER);
  shader->CreateShader("shaders\\fshader.frag.glsl", GL_FRAGMENT_SHADER);
  shader->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetAt(0, 0, 0);
  cam->SetEye(0, 0, 3);
  cam->SetupCamera();

  moon.Init(shader, cam);
  moon.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  moon.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
  moon.SetTextureAttribute("texcoord", 2, GL_ARRAY_BUFFER);
  moon.SetTangentAttribute("tan", 3, GL_ARRAY_BUFFER);
  moon.SetBitangentAttribute("bitan", 4, GL_ARRAY_BUFFER);
  moon.TransferData();
  moon.Translate(3, 1, -2);
  Manipulator::SetCurrent(moon.GetManipulator());

  moo.Init(shader, cam);
  moo.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  moo.SetNormalAttribute("normal", 1, GL_ARRAY_BUFFER);
  moo.SetTextureAttribute("texcoord", 2, GL_ARRAY_BUFFER);
  moo.SetTangentAttribute("tan", 3, GL_ARRAY_BUFFER);
  moo.SetBitangentAttribute("bitan", 4, GL_ARRAY_BUFFER);
  moo.TransferData();

  texmoon.Init("..\\textures\\moon.bmp");
  texmoon_norm.Init("..\\textures\\moonnorm.bmp");

  group = new Group();
  group->Init(shader, cam);
  group->AddChild(&moon);
  group->AddChild(&moo);
  
  //FramebufferInit();
}

void BlueSphereRender(glm::mat4 t)
{
  first_pass->UseProgram();
  glm::mat4 mvp = cam->m_proj * cam->m_view * t;
  first_pass->SetUniform("mvp", mvp);
  first_pass->SetUniform("m", t);
  first_pass->SetUniform("_m", glm::transpose(glm::inverse(t)));

  first_pass->SetUniform("mamb", RED * 0.2f);
  first_pass->SetUniform("mdiff", WHITE * 0.3f);
  first_pass->SetUniform("mspec", WHITE * 0.6f);
  first_pass->SetUniform("mshi", 128.0f);
  blue_sphere.Draw();
}

void DrawScene()
{
  //shader->UseProgram();
  //glm::mat4 model = moon.GetModel();
  //glm::mat4 mvp = cam->m_proj * cam->m_view * model;

  //shader->SetUniform("mvp", mvp);
  //shader->SetUniform("model", model);
  //shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));

  //shader->SetUniform("light", glm::vec3(-5, 0, 0));
  //shader->SetUniform("eye", cam->GetEye());

  //glm::vec3 white = glm::vec3(1, 1, 1);
  //shader->SetUniform("amb", white * 0.1f);
  //shader->SetUniform("diff", white * 1.0f);
  //shader->SetUniform("spec", white * 0.25f);
  //shader->SetUniform("shi", 10.0f);

  //texmoon_norm.LoadTexture();
  //shader->SetUniform("normtexture", texmoon_norm.m_id);
  //texmoon.LoadTexture();
  //shader->SetUniform("difftexture", texmoon.m_id);

  //moon.Draw();

  //model = moo.GetModel();
  //mvp = cam->m_proj * cam->m_view * model;

  //shader->SetUniform("mvp", mvp);
  //shader->SetUniform("model", model);
  //shader->SetUniform("tinv_model", glm::transpose(glm::inverse(model)));
  //moo.Draw();
  group->Translate(0, 0, -0.05);
  group->Draw();
}

void SceneRender()
{
  int l = NSPHERE;
  int n = l * l;
  for (int i = 0; i < n; i++)
  {
    float d = 2.5f;
    float z = ((i / l) - l / 2) * d;
    float x = ((i % l) - l / 2) * d;
    glm::mat4 t;
    t = glm::translate(t, glm::vec3(x, 0, z));
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

  second_pass->SetUniform("wrl_eye", cam->GetEye());
  second_pass->SetUniform("gamb", RED * 0.1f);

  int n = NLIGHTS / 2;
  for (GLuint i = 0; i < n; ++i)
  {
    float dt = (i * 2 * PI) / n;
    LIGHT_POS[i].x = 4 * sin(THETA + dt);
    LIGHT_POS[i].y = cam->GetEye().y;
    LIGHT_POS[i].z = 4 * cos(THETA + dt);

    LIGHT_SPOT[i].z = 4 * sin(dt);
    LIGHT_SPOT[i].y = 0;
    LIGHT_SPOT[i].x = 4 * cos(dt);
  }

  for (GLuint i = 0; i < n; ++i)
  {
    float dt = (i * 2 * PI) / n;
    LIGHT_POS[n+i].x = 8 * sin(THETA - dt);
    LIGHT_POS[n+i].y = cam->GetEye().y;
    LIGHT_POS[n+i].z = 8 * cos(THETA - dt);

    LIGHT_SPOT[n+i].z = 8 * sin(dt);
    LIGHT_SPOT[n+i].y = 0;
    LIGHT_SPOT[n+i].x = 8 * cos(dt);
  }

  second_pass->SetUniform("nlight", (int)NLIGHTS);
  second_pass->SetUniform("lpos", NLIGHTS, LIGHT_POS);

  second_pass->SetUniform("lamb", NLIGHTS, LIGHT_COLORS);
  second_pass->SetUniform("ldiff", NLIGHTS, LIGHT_COLORS);
  second_pass->SetUniform("lspec", NLIGHTS, LIGHT_COLORS);
  second_pass->SetUniform("lspot", NLIGHTS, LIGHT_SPOT);

  quad.Draw();

  if (THETA < 2 * PI)
    THETA += 0.05f;
  else
    THETA = 0.1f;
}

void DeferredDrawScene(/*Incluir ponteiro para funções*/)
{
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
  glDrawBuffers(5, draw_bufs);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);


  //* Scene Render
  SceneRender(); // Transformar em uma chama para ponteiro de função passado por parametro
  //*/

  // deferred pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  //* Quad Render with Texture
  QuadRender(); // Transformar em uma chama para ponteiro de função passado por parametro
  //*/
}

// Display callback
static void Display(void)
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // draw scene
  DrawScene();
  
  glutSwapBuffers();
  glutPostRedisplay();
}

// Main function
int main (int argc, char* argv[])
{
  srand(time(NULL));
  // open GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(W, H);

  // create window
  glutCreateWindow("Deferred Shading");
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);

  // initiate OpenGL context
  Init();

  // interact...
  glutMainLoop();
  return 0;
}