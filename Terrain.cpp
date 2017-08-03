
#include "Terrain.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/gl.h"
#include "Texture.h"
#include "opencv/highgui.h"
#include <opencv2/imgcodecs.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/vec2.hpp"
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

	glBindTexture(GL_TEXTURE_2D,m_iTextureIndex);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT, 0);
	int code = glGetError();
	const GLubyte  *buf = gluErrorString(code);
	buf = buf;


}

void Patch::Prepare()
{
}

void Patch::SampleTexture() {
	if (m_iTextureIndex <= 0)
	{
		return ;
	}

}


Terrain::Terrain()
{
	centerx = centery = centerz = 0.0f;
	m_fPatchSize = 4;
	m_iXCount = m_iZCount = 20;
}
GLint g_iTexIndex = 0;
Texture g_Texture;
struct TerrainVertex
{
	glm::vec3 pos;
	glm::vec2 uv;
};
void Terrain::Init()
{
	centerx = 0;
	centery = 0;

	Mat mat;
	mat = imread("test.bmp",IMREAD_COLOR);
	g_Texture.FillWithMat(mat);
	shader.Build("shaders/terrain_shader.vert","shaders/terrain_shader.frag",{"vVertex","vUV"},{"viewProj","center","patchindex","textureMap"});
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1,&vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	TerrainVertex vert[4];

	vert[0].pos = vec3(0.0f, 0.0f, 0.0f);
	vert[1].pos = vec3(0.0f, 0.0f, 1.0f);
	vert[2].pos = vec3(1.0f, 0.0f, 1.0f);
	vert[3].pos = vec3(1.0f, 0.0f, 0.0f);
	vert[0].uv = glm::vec2(0.0f,0.0f);
	vert[1].uv = vec2(0.0f,1.0f);
	vert[2].uv = vec2(1.0f,1.0f);
	vert[3].uv = vec2(1.0f,0.0f);


	GLushort indices[6];

	int count = 0;

	//fill indices array
	GLushort* id=&indices[0];

	*id++ = 0; 	*id++ = 1; 	*id++ = 2;
	*id++ = 0; 	*id++ = 2; 	*id++ = 3;
	GLuint vboIndicesID = 0;
	glGenBuffers(1, &vboIndicesID);




	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, &indices[0], GL_STATIC_DRAW);
	//pass triangle verteices to buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert[0], GL_STATIC_DRAW);
	shader.Use();
	int stride = sizeof(TerrainVertex);
	glEnableVertexAttribArray(shader["vVertex"]);
	glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(shader["vUV"]);
	glVertexAttribPointer(shader["vUV"],2,GL_FLOAT,GL_FALSE,stride,(const void *)offsetof(TerrainVertex,uv));

	shader.UnUse();
	for (int z = 0; z < m_iZCount; ++z)
	{
		for (int x = 0; x < m_iXCount; ++x)
		{
			Patch *patch = new Patch(x,z);
			terrainPatchs.push_back(patch);
		}
	}
}

void Terrain::Draw(mat4 viewProj)
{
	glEnable(GL_TEXTURE_2D);
	// first render a big and gray  ground
	glBindVertexArray(vaoID);
	shader.Use();
	int code = glGetError();
	glUniform1i(shader("textureMap"), 0);
	code = glGetError();
	glUniformMatrix4fv(shader("viewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));


	glUniform3f(shader("center"),centerx,centery,centerz);
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		int ix = (*iter)->m_iPatchX;
		int iy = (*iter)->m_iPatchY;
		glUniform3f(shader("patchindex"),(*iter)->m_iPatchX * m_fPatchSize,(*iter)->m_iPatchY * m_fPatchSize,m_fPatchSize);
		(*iter)->Draw();
	}
	shader.UnUse();
	return;
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

Terrain::~Terrain() {
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		delete *iter;

	}
	terrainPatchs.clear();

}

void Terrain::setTexture(GLuint texid) {

	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		(*iter)->m_iTextureIndex = texid;

	}
}
