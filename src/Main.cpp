#include <stdio.h>
#include <time.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Sphere.h"
#include "Manipulator.h"
#include "Quad.h"
#include "Cube.h"
#include "PerlinNoise.h"

#include <glm\gtc\matrix_transform.hpp>

int W = 800;
int H = 600;
int B_SIZE = 150;
Camera* cam = NULL;
Manipulator* manip = NULL;
Sphere sphere;
Quad quad;
Quad reveal;

GLuint framebuffer;
GLuint noise_buff;

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

void FramebufferInit()
{
  //* FrameBuffer Init
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glEnable(GL_TEXTURE_3D);

  // Vertex Texture Init
  glGenTextures(1, &noise_buff);
  glBindTexture(GL_TEXTURE_3D, noise_buff);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, B_SIZE, B_SIZE, B_SIZE, 0, GL_RGBA, GL_FLOAT, PerlinNoise::Generate3DTexture(B_SIZE));
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach Texture and FrameBuffer
  //glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, noise_buff, 0, 0);
}

void FramebufferFinish()
{
  //* FrameBuffer Finish
  glDeleteFramebuffers(1, &framebuffer);
  glDeleteTextures(1, &noise_buff);
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

  first_pass = new ShaderProgram();
  first_pass->Init();
  first_pass->CreateShader("shaders\\noise.vert.glsl", GL_VERTEX_SHADER);
  first_pass->CreateShader("shaders\\noise.frag.glsl", GL_FRAGMENT_SHADER);
  first_pass->LinkProgram();

  second_pass = new ShaderProgram();
  second_pass->Init();
  second_pass->CreateShader("shaders\\procedural.vert.glsl", GL_VERTEX_SHADER);
  second_pass->CreateShader("shaders\\procedural.frag.glsl", GL_FRAGMENT_SHADER);
  second_pass->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetEye(0, 0, 3);
  cam->SetAt(0, 0, 0);
  cam->SetupCamera();

  sphere.Init(second_pass, cam);
  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.SetTextureAttribute("texcoord", 1, GL_ARRAY_BUFFER);
  sphere.TransferData();
  Manipulator::SetCurrent(sphere.GetManipulator());

  quad.Init(first_pass, cam);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.TransferData();

  reveal.Init(second_pass, cam);
  reveal.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  reveal.TransferData();
  //Manipulator::SetCurrent(reveal.GetManipulator());

  FramebufferInit();
}

void DeferredDrawScene(/*Incluir ponteiro para funções*/)
{
  // deferred pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  //* Quad Render with Texture
  second_pass->UseProgram();

  glActiveTexture(GL_TEXTURE0 + noise_buff);
  glBindTexture(GL_TEXTURE_3D, noise_buff);
  second_pass->SetUniform("noise_tex", noise_buff);

  glm::mat4 mv = cam->m_view * sphere.GetModel();
  glm::mat4 mvp = cam->m_proj * mv;
  second_pass->SetUniform("mv", mv);
  second_pass->SetUniform("mvp", mvp);
  sphere.Draw();

  //*/
}

// Display callback
static void Display(void)
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // draw scene
  DeferredDrawScene();
  
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