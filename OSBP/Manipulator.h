// manipulator.h
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#ifndef V_MANIPULATOR_H
#define V_MANIPULATOR_H

#include <glm\mat4x4.hpp>

class VManipulator
{
private:
	glm::mat4* m_matrix;
	float m_zcenter;
	VManipulator* m_next;
	static VManipulator* s_current;

public:
	static VManipulator* getCurrent ();
	static void setCurrent (VManipulator* manipulator);

	VManipulator(glm::mat4* matrix);
	void setZCenter (float zcenter);
	void centralize ();
	void decentralize ();
	void rotate (float angle, float rx, float ry, float rz);
	void scale (float sx, float sy, float sz);
	float getYRotation ();

};

#endif
