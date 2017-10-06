#pragma once
#include "GL/glut.h"
#include "GL/gl.h"
#include "opencv2/core.hpp"
class Texture
{
public:
	Texture();
	~Texture();
public:
	GLuint m_iTextureIndex;
public:
	void FillWithMat(const cv::Mat &mat);
	void FillWithColor(int width,int height,unsigned int color);
	void useTexture();
	bool LoadFromFile(const std::string &filename);
	public:
	void DetachTexture();
	void AttachTexture(GLuint texturename);
public:
	void SaveToFile(const std::string &filename);
	
	static GLuint CloneTexture(GLuint texid);
	private:
	void genTexture();
	int m_iWidth;
	int m_iHeight;
};

class RenderTexture : public Texture
{
public:
	void prepareDraw();
	void postDraw();


};
void CheckGLError();