
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

class CPatchSampler
{
public:
	CPatchSampler()
	{
		vaoID = 0;
		vbID = 0;
		ibID = 0;
	}
	void Init();
	void Fini();

protected:
	GLuint vaoID;
	GLuint vbID;
	GLuint ibID;
public:
	// left down ,right down, top right, left top
	// four positions
	void SetWorldPositions(vec3 *worldpos);
	void Draw();
protected:
	void UpdateVertexBuffer();
protected:
	struct SamplerPos_t
	{
		vec2 vertpos;
		vec3 worldpos;
	};
	SamplerPos_t vertbuf[4];
	GLSLShader shader;
};

CPatchSampler m_patchSampler;
void CPatchSampler::Init()
{
	shader.Build("shaders/Terrain_Sampler", {"vVertex","vWorldPos"}, {"viewProj","textureMap"});
	glGenBuffers(1, &vbID);
	glGenBuffers(1, &ibID);
	
	 

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID);
	GLushort indices[] = {0,1,2,0,2,3};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort),indices,GL_STATIC_DRAW);
	
	// left down ,right down, top right, left top
	vertbuf[0].vertpos = vec2(-1.0f,-1.0f);
	vertbuf[1].vertpos = vec2(1.0f, -1.0f);
	vertbuf[2].vertpos = vec2(1.0f, 1.0f);
	vertbuf[3].vertpos = vec2(-1.0f, 1.0f);
	UpdateVertexBuffer();

	


}

void CPatchSampler::Fini()
{
	
	if (vaoID > 0)
	{
		glDeleteVertexArrays(1, &vaoID);
		vaoID = 0;
	}
	if (vbID > 0)
	{
		glDeleteBuffers(1, &vbID);
		vbID = 0;
	}
	if (ibID > 0)
	{
		glDeleteBuffers(1, &ibID);
		ibID = 0;
	}
	shader.Fini();
}

void CPatchSampler::SetWorldPositions(vec3* worldpos)
{
	for (int i = 0; i < 4; ++i)
	{
		vertbuf[i].worldpos = worldpos[i];
	}
	UpdateVertexBuffer();
}

void CPatchSampler::Draw()
{
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER,vbID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void CPatchSampler::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertbuf), vertbuf, GL_STREAM_DRAW);
	
}


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

	m_fPatchSize = 8;
	m_iXCount = m_iZCount = 20;
    m_fTerrainSizeX = m_iXCount * m_fPatchSize;
    m_fTerrainSizeZ = m_iZCount * m_fPatchSize;
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
	m_patchSampler.Init();
	Mat mat;
	mat = imread("test.bmp",IMREAD_COLOR);
	g_Texture.FillWithMat(mat);
	shader.Build("shaders/terrain_shader.vert","shaders/terrain_shader.frag",{"vVertex","vUV"},{"viewProj","patchorigin","patchsize","textureMap"});
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
void Terrain::Fini()
{
	shader.Fini();
	m_patchSampler.Fini();
	if (vaoID > 0)
	{
		glDeleteVertexArrays(1, &vaoID);
		vaoID = 0;
	}
	if (vboID > 0)
	{
		glDeleteBuffers(1, &vboID);
		vboID = 0;
	}
	if (vboIndicesID > 0)
	{
		glDeleteBuffers(1, &vboIndicesID);
		vbo
	}
}
void Terrain::Draw(mat4 viewProj)
{
	//glEnable(GL_TEXTURE_2D);
	// first render a big and gray  ground
	glBindVertexArray(vaoID);
	shader.Use();
	int code = glGetError();
	glUniform1i(shader("textureMap"), 0);
	code = glGetError();
	glUniformMatrix4fv(shader("viewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));
	
	glUniform1f(shader("patchsize"),m_fPatchSize);
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		int ix = (*iter)->m_iPatchX;
		int iy = (*iter)->m_iPatchY;
		vec3 patchcenter;
		patchcenter = GetPatchOrigin(ix, iy);
		glUniform3fv(shader("patchorigin"), 1,value_ptr(patchcenter));
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

void Terrain::SetCenter(float x, float y, float z) {
	centerx = x;
	centery = y;
	centerz = z;
}

void Terrain::SetPatchSize(float fPatchSize) {
	m_fPatchSize = fPatchSize;
	updateTerrainSize();
}

void Terrain::updateTerrainSize() {
	m_fTerrainSizeZ = m_iZCount * m_fPatchSize;
	m_fTerrainSizeX = m_iXCount * m_fPatchSize;
}

void Terrain::SetPatchNum(int patchX, int patchZ) {
	m_iXCount = patchX;
	m_iZCount = patchZ;
	updateTerrainSize();
}
vec3 Terrain::GetPatchOrigin(int iPatchX, int iPatchZ)
{
	vec3 patchorigin;
	patchorigin.x = iPatchX * m_fPatchSize + centerx - m_fTerrainSizeX * 0.5f;
	patchorigin.y = 0.0;
	patchorigin.z = iPatchZ * m_fPatchSize + centerz - m_fTerrainSizeZ * 0.5f;
	return patchorigin;
}





void Terrain::SampleTexture(glm::mat4 viewProj, GLuint texid, int width, int height) {

	glBindVertexArray(vaoID);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texid);
	glUniform1i(shader("textureMap"), 0);
	glUniformMatrix4fv(shader("viewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));

	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		int ix = (*iter)->m_iPatchX;
		int iy = (*iter)->m_iPatchY;
		vec3 patchcenter;
		patchcenter = GetPatchOrigin(ix, iy);
		
		(*iter)->SampleTexture();
	}
	shader.UnUse();
	return;
}
