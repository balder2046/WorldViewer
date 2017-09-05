#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "GL/glew.h"
#include "GLSLShader.h"
#include <memory>
#include "RenderTextureFBO.h"

namespace TRN
{
	class Patch
	{
	public:

		Patch(int iPatchX,int iPatchY);
		void Draw();
        void SampleTexture();
	public:
		void Prepare();
	public:
		int m_iPatchX;
		int m_iPatchY;
		int m_iTextureIndex;
	};
	class CPatchSampler
	{
	public:
		CPatchSampler()
		{
			vaoID = 0;
			vbID = 0;
			ibID = 0;
			m_iTexWidth = 0;
			m_iTexHeight = 0;
		}
		void Init(int texwidth,int texheight);
		void Fini();

	protected:
		GLuint vaoID;
		GLuint vbID;
		GLuint ibID;
	public:
		// left down ,right down, top right, left top
		// four positions
		void SetWorldPositions(glm::vec3 *worldpos);
		void Draw();
		void Sample(glm::mat4 viewproj, glm::vec3 *worldpos, int texid);
	protected:
		void UpdateVertexBuffer();
	protected:
		struct SamplerPos_t
		{
			glm::vec2 vertpos;
			glm::vec3 worldpos;
		};
		SamplerPos_t vertbuf[4];
		GLSLShader shader;
		int m_iTexWidth;
		int m_iTexHeight;
		std::unique_ptr<RenderTextureFBO> m_renderTextureFBO;
	};

	class Terrain
	{
	public:

		Terrain();
		~Terrain();
	public:
		void Init();
		void Fini();
		void Draw(glm::mat4 viewporj);
        void SampleTexture(glm::mat4 viewporj,GLuint texid,int width,int height);
	public:
		void TestTransform(glm::mat4 viewproj, glm::vec3 *corners,glm::vec2 *corners2d);
		void TestDraw(glm::mat4 viewporj);
	public:
		void setTexture(GLuint texid);

        void SetCenter(float x,float y,float z);
        void SetPatchSize(float fPatchSize);
        void SetPatchNum(int patchX,int patchZ);
	private:
		float centerx;
		float centery;
		float centerz;
	public:
		glm::vec3 GetPatchOrigin(int iPatchX, int iPatchZ);
		// 取四个顶点 ,left-bottom, right-bottom, top-bottom, left-top
		void GetPatchWorldCorners(int iPatchX, int iPatchZ,glm::vec3 *corners);
	private:
		float m_fPatchSize;
        float m_fTerrainSizeX;
        float m_fTerrainSizeZ;
        void updateTerrainSize();
	public:
		void toLocalPos(float &x,float &y, float &z);
		void toWorldPos(float &x, float &y, float &z);
	public:

	public:
		float getCenterX();
		float getCenterY();
	public:
		GLuint vaoID;
		GLuint vboID;
		GLuint iboID;
		GLSLShader shader;
		int m_iXCount;
		int m_iZCount;
		std::vector<Patch *> terrainPatchs;
		CPatchSampler m_patchSampler;

	};

}
