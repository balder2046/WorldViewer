#include "stdafx.h"
#include <GL/glew.h>
#include "Texture.h"
#include <vector>

using namespace cv;

Texture::Texture()
{
	m_iTextureIndex = 0;
}

void Texture::FillWithMat(const cv::Mat& mat)
{
	if (mat.channels() != 3) return;
	genTexture();
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
	glTexImage2D(GL_TEXTURE_2D, 0, 3, mat.cols , mat.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, &vecBuffer[0]);
	int code = glGetError();
	char *buf = (char *)gluErrorString(code);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::useTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_iTextureIndex);
}

void Texture::genTexture()
{
	if (m_iTextureIndex == 0)
	glGenTextures(1, &m_iTextureIndex);
}
