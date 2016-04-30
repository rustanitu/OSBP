#ifndef CAMERA_H
#define CAMERA_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Camera
{
public:
  Camera(int width, int height);

  // Camera Setup
  void SetAngle(float a);
  void SetZPlanes(float zn, float zf);
  void SetEye(float ex, float ey, float ez);
  void SetUp(float ux, float uy, float uz);
  void SetAt(float ax, float ay, float az);
  void SetViewport(int width, int height);
  void SetupCamera();

  // Getters
  glm::vec3 GetEye()
  {
    return m_eye;
  }

  // Camera Trsnformations
  void Translate(float tx, float ty, float tz);
  void Translate(glm::vec3 t);
  void Rotate(float a, float tx, float ty, float tz);
  void Rotate(float a, glm::vec3 t);
  void Scale(float sx, float sy, float sz);
  void Scale(glm::vec3 s);

private:
  // Camera Parameters
  glm::vec3 m_eye;
  glm::vec3 m_at;
  glm::vec3 m_up;
  float m_fov;
  float m_near;
  float m_far;

  // Screen Parameters
  int m_width;
  int m_height;

public:
  // Description Matrices
  glm::mat4 m_view;
  glm::mat4 m_proj;
};

#endif