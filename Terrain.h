﻿#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "GL/glew.h"
#include "GLSLShader.h"

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
		void SetWorldPositions(glm::vec3 *worldpos);
		void Draw();
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
