// manipulator.h
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#ifndef V_MANIPULATOR_H
#define V_MANIPULATOR_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#include "Camera.h"

class Manipulator
{
private:
	static Manipulator* s_current;
  Camera* m_cam;
	glm::mat4* m_matrix;

public:

	static Manipulator* GetCurrent ();
	static void SetCurrent (Manipulator* manipulator);

  Manipulator(glm::mat4* matrix);
	void Rotate (float angle, float rx, float ry, float rz);
	void Scale (float sx, float sy, float sz);

  glm::vec3 GetTargetPosition();

  void SetCamera(Camera* cam)
  {
    m_cam = cam;
  }

  Camera* GetCamera()
  {
    return m_cam;
  }

  glm::mat4 GetTargetBase()
  {
    return *m_matrix;
  }
};

#endif
