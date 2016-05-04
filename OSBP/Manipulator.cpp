// manipulator.cpp
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, set 2010

#include "Manipulator.h"
#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#define PI 3.1459
#define TORAD(x) (x*180.0f/PI)

VManipulator* VManipulator::s_current = 0;

// internal control
static char g_state = ' ';
static int g_x0, g_y0;

static glm::vec3 map (int x, int y)
{
	glm::vec3 v;
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	int radius = vp[2] < vp[3] ? vp[2]/2 : vp[3]/2;
	v.x = (float)((x-vp[0]) - vp[2]/2) / radius;
	v.y = (float)((vp[3]-y-vp[0]) - vp[3]/2) / radius;
	float len = sqrt(v.x*v.x + v.y*v.y);
	if (len > 1.0) {
		v.x /= len;
		v.y /= len;
		v.z = 0.0f;
	}
	else {
		v.z = sqrt(1 - v.x*v.x - v.y*v.y);
	}
	return v;
}

static void mouse (int button, int state, int x, int y)
{
	if (!VManipulator::getCurrent())
		return;
	if (state == GLUT_DOWN) {
		g_x0 = x; g_y0 = y;
		if (button == GLUT_LEFT_BUTTON)
			g_state = 'r';
		else if (button == GLUT_RIGHT_BUTTON)
			g_state = 's';
		else
			g_state = ' ';
	}
}

static void motion (int x, int y)
{
	if (!VManipulator::getCurrent())
		return;
	int dx = abs(x-g_x0);
	int dy = abs(y-g_y0);
	if ( dx > 1 || dy > 1) {
		if (g_state == 'r') {
			glm::vec3 v0 = map(g_x0, g_y0);
			glm::vec3 v1 = map(x, y);
			glm::vec3 r = glm::cross(v0, v1);
			VManipulator::getCurrent()->rotate(0.1f * TORAD(2*asin(glm::length(r))),r.x,r.y,r.z);
		}
		else if (g_state == 's') {
			int vp[4];
			glGetIntegerv(GL_VIEWPORT,vp);
			float f = dx > dy ? (float)(x-g_x0) / vp[2] : (float) (-y+g_y0) / vp[3];
			VManipulator::getCurrent()->scale(1+f, 1+f, 1+f);
		}
		g_x0 = x; g_y0 = y;
		glutPostRedisplay();
	}
}

void VManipulator::setCurrent (VManipulator* m_manipulator)
{
	s_current = m_manipulator;
}

VManipulator* VManipulator::getCurrent ()
{
	return s_current;
}

VManipulator::VManipulator(glm::mat4* matrix)
{
	m_matrix = matrix;
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	setCurrent(this);
	m_next = NULL;
  m_zcenter = 0.0f;
}

void VManipulator::setZCenter (float zcenter)
{
	m_zcenter = zcenter;
}

void VManipulator::rotate (float angle, float rx, float ry, float rz)
{
  *m_matrix = glm::rotate(*m_matrix, angle, glm::vec3(rx, ry, rz));
}

void VManipulator::scale (float sx, float sy, float sz)
{
  *m_matrix = glm::scale(*m_matrix, glm::vec3(sx, sy, sz));
}

void VManipulator::centralize ()
{
  *m_matrix = glm::translate(*m_matrix, glm::vec3(0, 0, -m_zcenter));
}

void VManipulator::decentralize ()
{
  *m_matrix = glm::translate(*m_matrix, glm::vec3(0, 0, m_zcenter));
}

float VManipulator::getYRotation ()
{
	float rad = -(*m_matrix)[2][0];
	return (rad==0) ? 0 : (rad/abs(rad))*asin(rad)*(360.0/(2*PI));
}