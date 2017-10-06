
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
#include "RenderTextureFBO.h"
using namespace TRN;
using namespace cv;
using namespace glm;



void CPatchSampler::Init(int texwidth,int texheight)
{
	m_iTexWidth = texwidth;
	m_iTexHeight = texheight;
	m_renderTextureFBO.reset(new RenderTextureFBO(texwidth, texheight));
	m_renderTextureFBO->Init();
	shader.Build("shaders/terrain_sampler", { "vVertex","vWorldPos" }, { "viewProj","textureMap" });
	glGenBuffers(1, &vbID);
	glGenBuffers(1, &ibID);



	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID);
	GLushort indices[] = { 0,1,2,0,2,3 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// left down ,right down, top right, left top
	vertbuf[0].vertpos = vec2(-1.0f, -1.0f);
	vertbuf[1].vertpos = vec2(1.0f, -1.0f);
	vertbuf[2].vertpos = vec2(1.0f, 1.0f);
	vertbuf[3].vertpos = vec2(-1.0f, 1.0f);
	UpdateVertexBuffer();

	// prepare attribe
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SamplerPos_t), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SamplerPos_t), (const void *)offsetof(SamplerPos_t, worldpos));
	glBindVertexArray(0);



}

void CPatchSampler::Fini()
{
	if (m_renderTextureFBO)
	{
		m_renderTextureFBO->Fini();
		m_renderTextureFBO.reset(0);
	}
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

void CPatchSampler::PreSample(GLuint texid,const mat4 &viewproj)
{
	shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glUniform1i(shader("textureMap"), 0);
	glUniformMatrix4fv(shader("viewProj"), 1, GL_FALSE, value_ptr(viewproj));
	glBindVertexArray(vaoID);
	
}
void CPatchSampler::PostSample()
{
	glBindVertexArray(0);
	shader.UnUse();
}

void CPatchSampler::Draw()
{
	shader.Use();
	glBindVertexArray(vaoID);
	//glBindBuffer(GL_ARRAY_BUFFER, vbID);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	shader.UnUse();
}

void CPatchSampler::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertbuf), vertbuf, GL_DYNAMIC_DRAW);

}
// texid 将采样的纹理放入texid
void CPatchSampler::Sample(glm::vec3 *worldpos, int texid)
{
	m_renderTextureFBO->Use();
	//glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetWorldPositions(worldpos);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	m_renderTextureFBO->UnUse();
	m_renderTextureFBO->CopyToTexture2(texid);
	m_renderTextureFBO->SaveTextureToFile(texid, "result.png");
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_iTextureIndex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	int code = glGetError();
	const GLubyte  *buf = gluErrorString(code);
	buf = buf;


}

void Patch::Prepare()
{
}



Terrain::Terrain()
{
	centerx = centery = centerz = 0.0f;

	m_fPatchSize = 8;
	m_iXCount = m_iZCount = 2;
	m_fTerrainSizeX = m_iXCount * m_fPatchSize;
	m_fTerrainSizeZ = m_iZCount * m_fPatchSize;
	vboID = 0;
	iboID = 0;
	vaoID = 0;
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
	m_patchSampler.Init(256,256);
	Mat mat;
	mat = imread("test.bmp", IMREAD_COLOR);
	g_Texture.FillWithMat(mat);
	shader.Build("shaders/terrain_shader.vert", "shaders/terrain_shader.frag", { "vVertex","vUV" }, { "viewProj","patchorigin","patchsize","textureMap" });
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);


	TerrainVertex vert[4];
	vert[0].pos = vec3(0.0f, 0.0f, 0.0f);
	vert[0].uv = glm::vec2(0.0f,0.0f);
	
	vert[1].pos = vec3(0.0f, 0.0f, 1.0f);
	vert[1].uv = vec2(0.0f, 1.0f);
	
	vert[2].pos = vec3(1.0f, 0.0f, 1.0f);
	vert[2].uv = vec2(1.0f, 1.0f);

	vert[3].pos = vec3(1.0f, 0.0f, 0.0f);
	vert[3].uv = vec2(1.0f, 0.0f);


	GLushort indices[6];

	int count = 0;

	//fill indices array
	GLushort* id = &indices[0];

	*id++ = 0; 	*id++ = 1; 	*id++ = 2;
	*id++ = 0; 	*id++ = 2; 	*id++ = 3;

	glGenBuffers(1, &iboID);




	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, &indices[0], GL_STATIC_DRAW);
	//pass triangle verteices to buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert[0], GL_STATIC_DRAW);
	shader.Use();
	int stride = sizeof(TerrainVertex);
	glEnableVertexAttribArray(shader["vVertex"]);
	glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(shader["vUV"]);
	glVertexAttribPointer(shader["vUV"], 2, GL_FLOAT, GL_FALSE, stride, (const void *)offsetof(TerrainVertex, uv));

	shader.UnUse();
	for (int z = 0; z < m_iZCount; ++z)
	{
		for (int x = 0; x < m_iXCount; ++x)
		{
			Patch *patch = new Patch(x, z);
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
	if (iboID > 0)
	{
		glDeleteBuffers(1, &iboID);
		iboID = 0;
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
	glUniform1f(shader("patchsize"), m_fPatchSize);
	glUniform1i(shader("flipy"), m_bFlipY);
	code = glGetError();
	glUniformMatrix4fv(shader("viewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		int ix = (*iter)->m_iPatchX;
		int iy = (*iter)->m_iPatchY;
		vec3 patchcenter;
		patchcenter = GetPatchOrigin(ix, iy);
		glUniform3fv(shader("patchorigin"), 1, value_ptr(patchcenter));
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

void Terrain::PreparePatchTextureFromFile(const std::string & filename)
{
	Texture tex;
	tex.LoadFromFile(filename);
	preparePatchTextureFromTexture(tex.m_iTextureIndex);
}
void Terrain::PreparePatchTextureFromColor(int width, int height, unsigned int color)
{
	Texture tex;
	tex.FillWithColor(width, height, color);
	preparePatchTextureFromTexture(tex.m_iTextureIndex);
}
void Terrain::preparePatchTextureFromTexture(GLuint texid)
{
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		
		(*iter)->m_iTextureIndex = Texture::CloneTexture(texid);

	}
}

void Terrain::setTexture(GLuint texid) {

	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		(*iter)->m_iTextureIndex = Texture::CloneTexture(texid);

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
Patch *Terrain::GetPatch(int patchx, int patchz)
{
	if (patchx < 0 || patchx >= m_iXCount) return 0;
	if (patchz < 0 || patchz >= m_iZCount) return 0;
	return terrainPatchs[patchz * m_iXCount + patchx];
}
vec3 Terrain::GetPatchOrigin(int iPatchX, int iPatchZ)
{
	vec3 patchorigin;
	patchorigin.x = iPatchX * m_fPatchSize + centerx - m_fTerrainSizeX * 0.5f;
	patchorigin.y = 0.0;
	patchorigin.z = iPatchZ * m_fPatchSize + centerz - m_fTerrainSizeZ * 0.5f;
	return patchorigin;
}



void Terrain::sampleTextureForPatch(Patch *pPatch)
{
	int ix = pPatch->m_iPatchX;
	int iy = pPatch->m_iPatchY;
	vec3 patchcenter;
	patchcenter = GetPatchOrigin(ix, iy);
	vec3 corners[4];
	GetPatchWorldCorners(ix, iy, corners);
	m_patchSampler.SetWorldPositions(corners);
	m_patchSampler.Sample(corners, pPatch->m_iTextureIndex);
}
void Terrain::SampleTexture(glm::mat4 viewporj, GLuint texid, int patchx, int patchz)
{
	glBindVertexArray(vaoID);
	m_patchSampler.PreSample(texid, viewporj);
	
	Patch *pPatch = GetPatch(patchx,patchz);
	sampleTextureForPatch(pPatch);
	
	m_patchSampler.PostSample();

}
void Terrain::SampleTexture(glm::mat4 viewProj, GLuint texid) {

	glBindVertexArray(vaoID);
	m_patchSampler.PreSample(texid, viewProj);
	for (auto iter = terrainPatchs.begin(); iter != terrainPatchs.end(); ++iter)
	{
		Patch *pPatch = *iter;
		sampleTextureForPatch(pPatch);
	}
	m_patchSampler.PostSample();
	
	return;
}
void Terrain::TestDraw(glm::mat4 viewporj)
{
	//we try to samle 1
	m_patchSampler.Draw();

}

void Terrain::TestTransform(glm::mat4 viewproj, glm::vec3 *corners, glm::vec2 *corners2d)
{
	// 取四个顶点 ,left-bottom, right-bottom, top-bottom, left-top
	
	
	bool b3DMode = false;
	
	for (int  i = 0;i < 4; i++)
	{
		vec4 pos = vec4(corners[i], 1.0f);
		pos = viewproj * pos;

		float u = (pos.x / pos.w + 1) * 0.5f;
		float v = 1 - (pos.y / pos.w + 1) * 0.5f;
		// transform to 0,1
		//get the viewport posfrom the uv
		corners2d[i].x = 2 * u - 1;
		corners2d[i].y = 1 - 2 * v;

	}

}
void Terrain::GetPatchWorldCorners(int x,int y,glm::vec3 *corners)
{
	// 取四个顶点 ,left-bottom, right-bottom, top-bottom, left-top
	vec3 patchcenter = GetPatchOrigin(x, y);
	corners[0] = glm::vec3(.0f, .0, m_fPatchSize) + patchcenter ;
	corners[1] = glm::vec3(m_fPatchSize, .0, m_fPatchSize) + patchcenter;
	corners[2] = glm::vec3(m_fPatchSize, .0, 0.0) + patchcenter;
	corners[3] = patchcenter;


}