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
Camera* cam = NULL;
Manipulator* manip = NULL;
Sphere sphere;
Quad quad(5);
static float s_time = 0.7;

GLuint planet_noise_buff;
GLuint planet_noise_buff_size = 300;
GLuint sky_noise_buff;
GLuint sky_noise_buff_size = 600;

ShaderProgram* planet_shader = NULL;
ShaderProgram* sky_shader = NULL;

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

// Reshape callback
static void Reshape(int w, int h)
{
  W = w;
  H = h;
  quad = Quad(W);
  quad.Init(sky_shader, cam);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.TransferData();
  glViewport(0, 0, w, h);
  cam->SetViewport(w, h);
  cam->SetupCamera();
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

  glEnable(GL_TEXTURE_3D);
  glGenTextures(1, &planet_noise_buff);
  glBindTexture(GL_TEXTURE_3D, planet_noise_buff);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, planet_noise_buff_size, planet_noise_buff_size, planet_noise_buff_size, 0, GL_RGBA, GL_FLOAT, PerlinNoise::Generate3DTexture(planet_noise_buff_size));
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &sky_noise_buff);
  glBindTexture(GL_TEXTURE_2D, sky_noise_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, sky_noise_buff_size, sky_noise_buff_size, 0, GL_RGBA, GL_FLOAT, PerlinNoise::Generate2DTexture(sky_noise_buff_size));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  planet_shader = new ShaderProgram();
  planet_shader->Init();
  planet_shader->CreateShader("shaders\\planet.vert.glsl", GL_VERTEX_SHADER);
  planet_shader->CreateShader("shaders\\planet.frag.glsl", GL_FRAGMENT_SHADER);
  planet_shader->LinkProgram();

  sky_shader = new ShaderProgram();
  sky_shader->Init();
  sky_shader->CreateShader("shaders\\sky.vert.glsl", GL_VERTEX_SHADER);
  sky_shader->CreateShader("shaders\\sky.frag.glsl", GL_FRAGMENT_SHADER);
  sky_shader->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetEye(0, 0, 6);
  cam->SetAt(0, 0, 0);
  cam->SetupCamera();

  sphere.Init(planet_shader, cam);
  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.TransferData();
  Manipulator::SetCurrent(sphere.GetManipulator());

  quad.Init(sky_shader, cam);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.TransferData();
  //Manipulator::SetCurrent(quad.GetManipulator());
}

void RenderScene()
{
  sky_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + sky_noise_buff);
  glBindTexture(GL_TEXTURE_2D, sky_noise_buff);
  sky_shader->SetUniform("noise_tex", sky_noise_buff);

  glm::mat4 mvp = cam->m_proj * cam->m_view * quad.GetModel();
  sky_shader->SetUniform("mvp", mvp);
  sky_shader->SetUniform("size", (float)5);
  quad.Draw();

  planet_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + planet_noise_buff);
  glBindTexture(GL_TEXTURE_3D, planet_noise_buff);
  planet_shader->SetUniform("noise_tex", planet_noise_buff);

  mvp = cam->m_proj * cam->m_view * sphere.GetModel();
  planet_shader->SetUniform("mvp", mvp);
  planet_shader->SetUniform("time", s_time);
  sphere.Draw();
}

// Display callback
static void Display(void)
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // draw scene
  RenderScene();
  
  glutSwapBuffers();
  glutPostRedisplay();
}

void TimeStep(int val)
{
  s_time += 0.005;
  if (s_time > 1.0f)
    s_time -= 0.3f;
  glutTimerFunc(val, TimeStep, val);
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
  glutCreateWindow("Noise");
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);

  // initiate OpenGL context
  Init();
  int val = 75;
  glutTimerFunc(val, TimeStep, val);

  // interact...
  glutMainLoop();
  return 0;
}