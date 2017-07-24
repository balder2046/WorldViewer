#pragma once
#include "GL/glut.h"
#include "GL/gl.h"
#include "opencv2/core.hpp"
class Texture
{
public:
	Texture();
public:
	GLuint m_iTextureIndex;
public:
	void FillWithMat(const cv::Mat &mat);
	void useTexture();
private:
	void genTexture();
};

class RenderTexture : public Texture
{
public:
	void prepareDraw();
	void postDraw();


};
