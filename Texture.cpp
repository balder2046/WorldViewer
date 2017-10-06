#include <GL/glew.h>
#include "Texture.h"
#include <vector>
#include <opencv2/imgcodecs.hpp>

using namespace cv;

Texture::Texture()
{
	m_iTextureIndex = 0;
	m_iWidth = 0;
	m_iHeight = 0;
}
Texture::~Texture()
{
	if (m_iTextureIndex > 0)
	{
		glDeleteTextures(1, &m_iTextureIndex);
		m_iTextureIndex = 0;
	}
}

void Texture::FillWithMat(const cv::Mat& mat)
{
	if (mat.channels() != 3) return;
	genTexture();
	m_iWidth = mat.cols;
	m_iHeight = mat.rows;
	glBindTexture(GL_TEXTURE_2D, m_iTextureIndex);

	std::vector<unsigned char> vecBuffer;
	vecBuffer.resize(mat.cols * mat.rows * 3);
	int index = 0;
	for (int iY = 0; iY < mat.rows; ++iY)
	{
		int y = iY;
		const Vec3b *buf = mat.ptr<Vec3b>(y);
		for (int iX = 0; iX < mat.cols; ++iX,++index)
		{
			vecBuffer[3 * index] = buf[iX][0];
			vecBuffer[3 * index + 1] = buf[iX][1];
			vecBuffer[3 * index + 2] = buf[iX][2];
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols , mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, &vecBuffer[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::FillWithColor(int width,int height,unsigned int color)
{
	genTexture();
	m_iWidth = width;
	m_iHeight = height;
	unsigned char r, g, b;
	r = color & 0xff;
	g = (color >> 8) & 0xff;
	b = (color >> 16) & 0xff;
	glBindTexture(GL_TEXTURE_2D, m_iTextureIndex);

	std::vector<unsigned char> vecBuffer;
	vecBuffer.resize(width * height * 3);
	int index = 0;
	for (int iY = 0; iY < height; ++iY)
	{
		int y = iY;
		
		for (int iX = 0; iX < width; ++iX, ++index)
		{
			vecBuffer[3 * index] = r;
			vecBuffer[3 * index + 1] = g;
			vecBuffer[3 * index + 2] = b;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &vecBuffer[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
bool Texture::LoadFromFile(const std::string &filename)
{
	if (m_iTextureIndex > 0)
	{
		glDeleteTextures(1, &m_iTextureIndex);
		m_iTextureIndex = 0;
	}
	Mat mat;
	mat = imread(filename.c_str(), IMREAD_COLOR);

	
	FillWithMat(mat);
	return true;
	
}

void Texture::useTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_iTextureIndex);
}

void Texture::genTexture()
{
	if (m_iTextureIndex > 0)
	{
		glDeleteTextures(1, &m_iTextureIndex);
		m_iTextureIndex = 0;
	}
	if (m_iTextureIndex == 0)
	glGenTextures(1, &m_iTextureIndex);
}

void Texture::DetachTexture() {
	if (m_iTextureIndex > 0)
	{
		glDeleteTextures(1,&m_iTextureIndex);
		m_iTextureIndex = 0;
	}

}
GLuint Texture::CloneTexture(GLuint texid)
{
	glBindTexture(GL_TEXTURE_2D, texid);
	int width, height;
	width = height = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	GLuint fbo = 0;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
	
	GLuint newtexid = 0;
	glGenTextures(1, &newtexid);
	glBindTexture(GL_TEXTURE_2D, newtexid);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, width, height, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
	return newtexid;
}
void Texture::AttachTexture(GLuint texturename) {
	DetachTexture();
	m_iTextureIndex = texturename;
}

void Texture::SaveToFile(const std::string &filename) {


}

void CheckGLError() {
	int code = glGetError();
	char *buf = (char *)gluErrorString(code);
	buf = buf;
	printf("%s",buf);
    if (code > 0)
    assert(false);
}
