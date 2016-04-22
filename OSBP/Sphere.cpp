#include"Sphere.h"
#include"Material.h"

Sphere::Sphere(float radius, int nslices, int nstacks)
{
	int i, j;
	float theta, phi, tx_theta, tx_phi;
	float x, y, z;

	tx_theta = (2 * PI) / nslices;
	tx_phi = (PI) / nstacks;
	float ***m = (float***)malloc((nstacks + 1)*sizeof(float**));
	for (i = 0; i < nstacks; i++)
	{
		phi = tx_phi*i;
		m[i] = (float**)malloc((nslices + 1)*sizeof(float*));
		for (j = 0; j < nslices; j++)
		{
			m[i][j] = (float*)malloc(3 * sizeof(float));
			theta = tx_theta*j;
			x = radius*cos(theta)*sin(phi);
			y = radius*cos(phi);
			z = radius*sin(theta)*sin(phi);
			m[i][j][0] = x;
			m[i][j][1] = y;
			m[i][j][2] = z;
		}
		m[i][j] = (float*)malloc(3 * sizeof(float));
		theta = 0;
		x = radius*cos(theta)*sin(phi);
		y = radius*cos(phi);
		z = radius*sin(theta)*sin(phi);
		m[i][j][0] = x;
		m[i][j][1] = y;
		m[i][j][2] = z;
	}
	phi = tx_phi*i;
	m[i] = (float**)malloc((nslices + 1)*sizeof(float*));
	for (j = 0; j < nslices; j++)
	{
		m[i][j] = (float*)malloc(3 * sizeof(float));
		theta = tx_theta*j;
		x = radius*cos(theta)*sin(phi);
		y = radius*cos(phi);
		z = radius*sin(theta)*sin(phi);
		m[i][j][0] = x;
		m[i][j][1] = y;
		m[i][j][2] = z;
	}
	m[i][j] = (float*)malloc(3 * sizeof(float));
	theta = 0;
	x = radius*cos(theta)*sin(phi);
	y = radius*cos(phi);
	z = radius*sin(theta)*sin(phi);
	m[i][j][0] = x;
	m[i][j][1] = y;
	m[i][j][2] = z;

	total = nslices*(nstacks)* 2;
	face = (Face**)malloc(total*sizeof(Face*));
	int k = 0;
	for (i = 0; i<nslices; i++)
	{
		for (j = 0; j<nstacks; j++)
		{
			face[k++] = new Face(m[j][i], m[j][i + 1], m[j + 1][i]);
			face[k++] = new Face(m[j + 1][i], m[j][i + 1], m[j + 1][i + 1]);
		}
	}
}

// draw sphere
void Sphere::draw(unsigned int primitive)
{
	glBegin(GL_TRIANGLES);
	int i = 0;
	while (i < total)
	{
		glNormal3fv(face[i]->Vi);
		glVertex3fv(face[i]->Vi);
		glNormal3fv(face[i]->Vj);
		glVertex3fv(face[i]->Vj);
		glNormal3fv(face[i]->Vk);
		glVertex3fv(face[i]->Vk);
		i++;
	}
	glEnd();
}