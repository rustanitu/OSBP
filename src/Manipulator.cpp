#include "Manipulator.h"
#include <glm\gtc\matrix_transform.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#define PI 3.14159265359

Manipulator* Manipulator::s_current = 0;

enum ACTION
{
	NO_ACTION,
	XY_ROTATION,
	Z_ROTATION,
	SCALE,
};

// internal control
static ACTION s_state = NO_ACTION;
static int s_x0, s_y0;

static void mouse (int button, int state, int x, int y)
{
	if (!Manipulator::GetCurrent())
		return;

	if (state == GLUT_DOWN) {
		s_x0 = x; s_y0 = y;
		if (button == GLUT_LEFT_BUTTON)
			s_state = XY_ROTATION;
		else if (button == GLUT_RIGHT_BUTTON)
			s_state = Z_ROTATION;
		else if (button == GLUT_MIDDLE_BUTTON)
			s_state = SCALE;
		else
			s_state = NO_ACTION;
	}
}

static void motion (int x, int y)
{
	if (!Manipulator::GetCurrent())
		return;

	int dx = abs(x-s_x0);
	int dy = abs(y-s_y0);
	if ( dx > 1 || dy > 1)
	{
		Manipulator* manip = Manipulator::GetCurrent();
		int vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);

		Camera* cam = manip->GetCamera();
		glm::vec3 targetpos = manip->GetTargetPosition();
		float dist = glm::distance(cam->GetEye(), targetpos);
		dist -= cam->GetNear();

		glm::mat4 base = manip->GetTargetBase();

		float theta;
		glm::vec4 axis;
		if (s_state == XY_ROTATION)
		{
			if (dx > dy)
			{
				theta = (x - s_x0) * PI * dist / (vp[2] - vp[0]);
				manip->Rotate(theta, base[0][1], base[1][1], base[2][1]);
			}
			else
			{
				theta = (y - s_y0) * PI * dist / (vp[3] - vp[1]);
				manip->Rotate(theta, base[0][0], base[1][0], base[2][0]);
			}
		}
		else if (s_state == Z_ROTATION)
		{
			if (dx > dy)
				theta = (s_x0 - x)* PI * dist / (vp[2] - vp[0]);
			else
				theta = (s_y0 - y)* PI * dist / (vp[3] - vp[1]);
			manip->Rotate(theta, base[0][2], base[1][2], base[2][2]);
		}
		else if (s_state == SCALE)
		{
			int vp[4];
			glGetIntegerv(GL_VIEWPORT,vp);
			float f = dx > dy ? (float)(x - s_x0) / vp[2] : (float)(s_y0 - y) / vp[3];
			manip->Scale(1+f, 1+f, 1+f);
		}
		s_x0 = x; s_y0 = y;
		glutPostRedisplay();
	}
}

void Manipulator::SetCurrent (Manipulator* m_manipulator)
{
	s_current = m_manipulator;
}

Manipulator* Manipulator::GetCurrent ()
{
	return s_current;
}

Manipulator::Manipulator(glm::mat4* matrix)
{
	m_matrix = matrix;
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
}

void Manipulator::Rotate (float angle, float rx, float ry, float rz)
{
	*m_matrix = glm::rotate(*m_matrix, angle, glm::vec3(rx, ry, rz));
}

void Manipulator::Scale (float sx, float sy, float sz)
{
	*m_matrix = glm::scale(*m_matrix, glm::vec3(sx, sy, sz));
}

glm::vec3 Manipulator::GetTargetPosition()
{
	return glm::mat3((*m_matrix)) * glm::vec3(0, 0, 0);
}
