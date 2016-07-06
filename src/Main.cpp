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
#include <iostream>

#include <glm\gtc\matrix_transform.hpp>

int W = 800;
int H = 600;
Camera* cam = NULL;
Sphere sphere;
const float quad_size = 15;
const float quad_light_size = 10;
Quad quad(quad_size);
Quad corona(3);
Quad sun_light(quad_light_size);
static float s_time = 0.7;
static float s_delta = 0.005;

GLuint planet_noise_buff;
GLuint planet_noise_buff_size = 256;
float* planet_buff = NULL;
GLuint sky_noise_buff;
GLuint sky_noise_buff_size = 1024;
float* sky_buff = NULL;

ShaderProgram* sun_shader = NULL;
ShaderProgram* sun_light_shader = NULL;
ShaderProgram* sky_shader = NULL;
ShaderProgram* corona_shader = NULL;

glm::mat4 turb_rot;

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

  planet_buff = PerlinNoise::Generate3DTexture(planet_noise_buff_size);
  glEnable(GL_TEXTURE_3D);
  glGenTextures(1, &planet_noise_buff);
  glBindTexture(GL_TEXTURE_3D, planet_noise_buff);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, planet_noise_buff_size, planet_noise_buff_size, planet_noise_buff_size, 0, GL_RGBA, GL_FLOAT, planet_buff);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  sky_buff = PerlinNoise::Generate2DTexture(sky_noise_buff_size);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &sky_noise_buff);
  glBindTexture(GL_TEXTURE_2D, sky_noise_buff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, sky_noise_buff_size, sky_noise_buff_size, 0, GL_RGBA, GL_FLOAT, sky_buff);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  sun_shader = new ShaderProgram();
  sun_shader->Init();
  sun_shader->CreateShader("shaders\\sun.vert.glsl", GL_VERTEX_SHADER);
  sun_shader->CreateShader("shaders\\sun.frag.glsl", GL_FRAGMENT_SHADER);
  sun_shader->LinkProgram();

  sun_light_shader = new ShaderProgram();
  sun_light_shader->Init();
  sun_light_shader->CreateShader("shaders\\sunlight.vert.glsl", GL_VERTEX_SHADER);
  sun_light_shader->CreateShader("shaders\\sunlight.frag.glsl", GL_FRAGMENT_SHADER);
  sun_light_shader->LinkProgram();

  corona_shader = new ShaderProgram();
  corona_shader->Init();
  corona_shader->CreateShader("shaders\\corona.vert.glsl", GL_VERTEX_SHADER);
  corona_shader->CreateShader("shaders\\corona.frag.glsl", GL_FRAGMENT_SHADER);
  corona_shader->LinkProgram();

  sky_shader = new ShaderProgram();
  sky_shader->Init();
  sky_shader->CreateShader("shaders\\sky.vert.glsl", GL_VERTEX_SHADER);
  sky_shader->CreateShader("shaders\\sky.frag.glsl", GL_FRAGMENT_SHADER);
  sky_shader->LinkProgram();
  
  cam = new Camera(W, H);
  cam->SetEye(0, 0, 6);
  cam->SetAt(0, 0, 0);
  cam->SetupCamera();

  quad.Init(sky_shader, cam);
  quad.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  quad.TransferData();

  sphere.Init(sun_shader, cam);
  sphere.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sphere.TransferData();
  Manipulator::SetCurrent(sphere.GetManipulator());

  sun_light.Init(sun_light_shader, cam);
  sun_light.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  sun_light.TransferData();
  sun_light.Translate(0, 0, 0.1f);

  corona.Init(corona_shader, cam);
  corona.SetVertexAttribute("vertex", 0, GL_ARRAY_BUFFER);
  corona.TransferData();
  corona.Translate(0, 0, 0.2f);
}

void RenderScene()
{
  glm::mat4 mvp;

  // Sky Render
  sky_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + sky_noise_buff);
  glBindTexture(GL_TEXTURE_2D, sky_noise_buff);
  sky_shader->SetUniform("noise_tex", sky_noise_buff);

  mvp = cam->m_proj * cam->m_view * quad.GetModel();
  sky_shader->SetUniform("mvp", mvp);
  sky_shader->SetUniform("size", quad_size);
  quad.Draw();

  
  // Sun Render
  sun_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + planet_noise_buff);
  glBindTexture(GL_TEXTURE_3D, planet_noise_buff);
  sun_shader->SetUniform("noise_tex", planet_noise_buff);

  mvp = cam->m_proj * cam->m_view * sphere.GetModel();
  sun_shader->SetUniform("mvp", mvp);
  sun_shader->SetUniform("time", s_time);
  sphere.Draw();


  // Sun Light Render
  sun_light_shader->UseProgram();
  mvp = cam->m_proj * cam->m_view * sun_light.GetModel();
  sun_light_shader->SetUniform("mvp", mvp);
  sun_light_shader->SetUniform("size", quad_light_size / 2.0f);
  sun_light.Draw();


  // Corona Render
  corona_shader->UseProgram();

  glActiveTexture(GL_TEXTURE0 + planet_noise_buff);
  glBindTexture(GL_TEXTURE_3D, planet_noise_buff);
  corona_shader->SetUniform("turb_tex", planet_noise_buff);

  glActiveTexture(GL_TEXTURE0 + sky_noise_buff);
  glBindTexture(GL_TEXTURE_2D, sky_noise_buff);
  corona_shader->SetUniform("noise_tex", sky_noise_buff);

  corona_shader->SetUniform("turb_rot", turb_rot);
  mvp = cam->m_proj * cam->m_view * corona.GetModel();
  corona_shader->SetUniform("mvp", mvp);
  corona.Draw();
}

// Display callback
static void Display(void)
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // draw scene
  RenderScene();
  
  glutSwapBuffers();
  glutPostRedisplay();
}

void TimeStep(int val)
{
  s_time += s_delta;
  
  if (s_time > 1.0f || s_time < 0.7)
  {
    s_delta = -s_delta;
    s_time += 2*s_delta;
  }

  turb_rot = glm::rotate(turb_rot, 0.02f, glm::vec3(1, 0, 0));

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
  int val = 100;
  glutTimerFunc(val, TimeStep, val);

  // interact...
  glutMainLoop();

  delete[] planet_buff;
  delete[] sky_buff;

  delete sun_shader;
  delete sky_shader;
  delete cam;

  return 0;
}