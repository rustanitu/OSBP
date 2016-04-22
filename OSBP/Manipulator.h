// manipulator.h
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#ifndef V_MANIPULATOR_H
#define V_MANIPULATOR_H

class VManipulator
{
private:
	float m_matrix[16];
	float m_invMatrix[16];
	float m_zcenter;
	VManipulator* m_next;
	static VManipulator* s_current;

public:
	static VManipulator* getCurrent ();
	static void setCurrent (VManipulator* manipulator);

	VManipulator ();
	void setZCenter (float zcenter);
	void load ();
	void unLoad ();
	void identity();
	void rotate (float angle, float rx, float ry, float rz);
	void scale (float sx, float sy, float sz);
	float getYRotation ();
	float* getInverseMatrix ();

};

#endif
