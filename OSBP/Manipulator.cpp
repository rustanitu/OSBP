// manipulator.cpp
// Visualiza��o 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, set 2010

#include "Manipulator.h"
#include "vector.h"

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

static VVector map (int x, int y)
{
	VVector v;
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
			VVector v0 = map(g_x0, g_y0);
			VVector v1 = map(x, y);
			VVector r = Cross(v0, v1);
			VManipulator::getCurrent()->rotate(TORAD(2*asin(r.length())),r.x,r.y,r.z);

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

VManipulator::VManipulator ()
{
	identity();
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	setCurrent(this);
	m_next = NULL;
}

void VManipulator::setZCenter (float zcenter)
{
	m_zcenter = zcenter;
}

void VManipulator::identity ()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_invMatrix);
	glPopMatrix();
	glPopAttrib();
}

void VManipulator::rotate (float angle, float rx, float ry, float rz)
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, rx, ry, rz);
	glMultMatrixf(m_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);

	glLoadIdentity();
	glMultMatrixf(m_invMatrix);
	glRotatef(-angle, rx, ry, rz);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_invMatrix);

	glPopMatrix();
	glPopAttrib();
}

void VManipulator::scale (float sx, float sy, float sz)
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(sx, sy, sz);
	glMultMatrixf(m_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);

	glLoadIdentity();
	glMultMatrixf(m_invMatrix);
	glScalef(1/sx, 1/sy, 1/sz);
	glGetFloatv(GL_MODELVIEW_MATRIX,m_invMatrix);

	glPopMatrix();
	glPopAttrib();
}

void VManipulator::load ()
{
	glTranslatef(0.0f, 0.0f, -m_zcenter);
	glMultMatrixf(m_matrix);
	glTranslatef(0.0f, 0.0f, m_zcenter);
}

float* VManipulator::getInverseMatrix ()
{
	return m_invMatrix;
}

void VManipulator::unLoad ()
{
	glTranslatef(0.0f, 0.0f, -m_zcenter);
	glMultMatrixf(m_invMatrix);
	glTranslatef(0.0f, 0.0f, m_zcenter);
}

float VManipulator::getYRotation ()
{
	float rad = -m_matrix[8];
	return (rad==0) ? 0 : (rad/abs(rad))*asin(rad)*(360.0/(2*PI));
}