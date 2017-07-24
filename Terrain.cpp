#include "stdafx.h"

#include "Terrain.h"
#include "gl/glut.h"
#include "gl/gl.h"
#include "Texture.h"
#include "opencv/highgui.h"
#include <opencv2/imgcodecs.hpp>
#include <glm/vec3.hpp>
using namespace TRN;
using namespace cv;
using namespace glm;
Patch::Patch(int iPatchX, int iPatchY)
{
	m_iPatchX = iPatchX;
	m_iPatchY = iPatchY;

}

void Patch::Draw()
{
	if (m_iTextureIndex <= 0)
	{
		return;
	}
}

void Patch::Prepare()
{
}


Terrain::Terrain()
{
	centerx = centery = centerz = 0.0f;
	m_fPatchSize = 4;
}
GLint g_iTexIndex = 0;
Texture g_Texture;
void Terrain::Init()
{
	Mat mat;
	mat = imread("test.bmp",IMREAD_COLOR);
	g_Texture.FillWithMat(mat);
	
}

void Terrain::Draw()
{
	// first render a big and gray  ground

	// blend a texture to file
	GLfloat vPos0[3] = {0.0f,0.0f,0.0f};
	GLfloat vPos1[3] = { 0.0f,0.0f,1.0f };
	GLfloat vPos2[3] = { 1.0f,0.0f,1.0f };
	GLfloat vPos3[3] = { 1.0f,0.0f,0.0f };
	glm::vec3 vPos[4];
	vPos[0] = vec3(0.0f, 0.0f, 0.0f);
	vPos[1] = vec3(0.0f, 0.0f, 1.0f);
	vPos[2] = vec3(1.0f, 0.0f, 1.0f);
	vPos[3] = vec3(1.0f, 0.0f, 0.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	g_Texture.useTexture();
	for (int i = -10; i < 10; ++i)
	{
		for (int j = -10; j < 10; ++j)
		{	
			
			glTranslatef(i, 0, j);
			glBegin(GL_QUADS);
			glColor4f(0.1f, 0.5f, 0.9f, 1.0f);
			glVertex3fv(vPos0);
			glTexCoord2f(.0, .0f);
			//glColor4f(0.1f, 0.5f, 0.9f, 1.0f);
			glVertex3fv(vPos1);
			glTexCoord2f(.0, 1.0f);
			//glColor4f(0.1f, 0.5f, 0.9f, 1.0f);
			glVertex3fv(vPos2);
			glTexCoord2f(1.0, 1.0f);
			//glColor4f(0.1f, 0.5f, 0.9f, 1.0f);
			glVertex3fv(vPos3);
			glTexCoord2f(1.0, 0.0f);
			glEnd();
			glTranslatef(-i, 0, -j);
			
		}
	}
	

	 
}

void Terrain::toLocalPos(float& x, float& y, float& z)
{
	x -= centerx;
	y -= centery;
	z -= centerz;
}

void Terrain::toWorldPos(float& x, float& y, float& z)
{
	x += centerx;
	y += centerz;
	z += centerz;
}

float Terrain::getCenterX()
{
	return centerx;
}
float Terrain::getCenterY()
{
	return centery;
}
