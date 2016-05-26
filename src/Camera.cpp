#include"Camera.h"
#include <stdio.h>

#include <glm\gtc\matrix_transform.hpp>

Camera::Camera(int width, int height)
:m_width(width), m_height(height)
{
  m_eye = glm::vec3(0, 0, 1);
  m_at = glm::vec3(0, 0, 0);
  m_up = glm::vec3(0, 1, 0);

  m_fov = 45;
  m_near = 1;
  m_far = 1000;
  SetupCamera();
}

/****************/
/* Camera Setup */
/****************/

void Camera::SetAngle(float a)
{
  m_fov = a;
}

void Camera::SetZPlanes(float zn, float zf)
{
  m_near = zn;
  m_far = zf;
}

void Camera::SetEye(float ex, float ey, float ez)
{
  m_eye.x = ex;
  m_eye.y = ey;
  m_eye.z = ez;
}

void Camera::SetUp(float ux, float uy, float uz)
{
  m_up.x = ux;
  m_up.y = uy;
  m_up.z = uz;
}

void Camera::SetAt(float ax, float ay, float az)
{
  m_at.x = ax;
  m_at.y = ay;
  m_at.z = az;
}

void Camera::SetViewport(int width, int height)
{
  m_width = width;
  m_height = height;
}

void Camera::SetupCamera()
{
  m_proj = glm::perspective(m_fov, (float)m_width / m_height, m_near, m_far);
  m_view = glm::lookAtRH(m_eye, m_at, m_up);
}

/**************************/
/* Camera Transformations */
/**************************/

void Camera::Translate(float tx, float ty, float tz)
{
  m_view = glm::translate(m_view, -glm::vec3(tx, ty, tz));
}

void Camera::Translate(glm::vec3 t)
{
  m_view = glm::translate(m_view, -t);
}

void Camera::Rotate(float a, float tx, float ty, float tz)
{
  m_view = glm::rotate(m_view, -a, glm::vec3(tx, ty, tz));
}

void Camera::Rotate(float a, glm::vec3 t)
{
  m_view = glm::rotate(m_view, -a, t);
}

void Camera::Scale(float sx, float sy, float sz)
{
  m_view = glm::scale(m_view, glm::vec3(1.0f / sx, 1.0f / sy, 1.0f / sz));
}

void Camera::Scale(glm::vec3 s)
{
  m_view = glm::scale(m_view, 1.0f / s);
}