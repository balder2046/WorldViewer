#pragma once

#include "glm/mat4x4.hpp"
#include "GL/glew.h"
#include "GLSLShader.h"

namespace TRN
{
	class Patch
	{
	public:

		Patch(int iPatchX,int iPatchY);
		void Draw();
	public:
		void Prepare();
	public:
		int m_iPatchX;
		int m_iPatchY;
		int m_iTextureIndex;
	};

	class Terrain
	{
	public:

		Terrain();
		~Terrain();
	public:
		void Init();
		void Draw(glm::mat4 viewporj);
	public:
		void setTexture(GLuint texid);
	private:
		float centerx;
		float centery;
		float centerz;
	private:
		float m_fPatchSize;
	public:
		void toLocalPos(float &x,float &y, float &z);
		void toWorldPos(float &x, float &y, float &z);
	public:
		float getCenterX();
		float getCenterY();
	public:
		GLuint vaoID;
		GLuint vboID;
		GLSLShader shader;
		int m_iXCount;
		int m_iZCount;
		std::vector<Patch *> terrainPatchs;


	};

}
