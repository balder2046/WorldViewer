#include "GL/glew.h"
#include "RenderTextureFBO.h"
#include "ScreenSizeQuad.h"
#include "ScreenSizeQuad.h"
#include "opencv2/imgcodecs.hpp"
RenderTextureFBO::RenderTextureFBO(int width, int height)
{
	m_iwidth = width;
	m_iheight = height;
	fboID = 0;
	for (int i = 0; i < 4; ++i)
	{
		m_size[i] = 0;
	}
}
RenderTextureFBO::RenderTextureFBO() {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	int width = viewport[2];
	int height = viewport[3];
	//RenderTextureFBO(width,height);
	m_iwidth = width;
	m_iheight = height;
	fboID = 0;
	for (int i = 0; i < 4; ++i)
	{
		m_size[i] = 0;
	}
}
CScreenSizeQuad *g_ScreenQuad = 0;
bool RenderTextureFBO::Init()
{
	if (g_ScreenQuad == 0)
	{
		g_ScreenQuad = new CScreenSizeQuad();
	}
	//generate and bind fbo ID
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);

	//generate and bind render buffer ID
	glGenRenderbuffers(1, &depthID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthID);

	//set the render buffer storage
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_iwidth, m_iheight);

	//generate the offscreen texture
	glGenTextures(1, &renderTextureID);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_iwidth, m_iheight, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	//bind the renderTextureID as colour attachment of FBO
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTextureID, 0);
	//set the render buffer as the depth attachment of FBO
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthID);

	//check for frame buffer completeness status
	GLuint status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO setup succeeded.");
	}
	else {
		printf("Error in FBO setup.");
	}

	//unbind the texture and FBO
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}
void RenderTextureFBO::Fini()
{
	glDeleteTextures(1, &renderTextureID);
	renderTextureID = 0;
	glDeleteRenderbuffers (1, &depthID);
	depthID = 0;
	glDeleteFramebuffers(1, &fboID);
	fboID = 0;
}

void RenderTextureFBO::Use()
{
	//enable FBO
	glGetIntegerv(GL_VIEWPORT,m_size);
	glViewport(0,0,m_iwidth,m_iheight);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
	//render to colour attachment 0
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
void RenderTextureFBO::UnUse()
{
	glViewport(m_size[0],m_size[1],m_size[2],m_size[3]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK_LEFT);
}
bool RenderTextureFBO::CopyToTexture(GLuint texid)
{
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
	glBindTexture(GL_TEXTURE_2D, texid);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);
	GLint size[4];
	glGetIntegerv(GL_VIEWPORT, size);
	glViewport(0, 0, m_iwidth, m_iheight);
	g_ScreenQuad->Render(NULL);
    glBindTexture(GL_TEXTURE_2D, renderTextureID);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTextureID, 0);
	glViewport(size[0], size[1], size[2], size[3]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return true;
}

bool RenderTextureFBO::CopyToTexture2(GLuint texid)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);
	glBindTexture(GL_TEXTURE_2D, texid);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, m_iwidth, m_iheight, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	return true;
}

void RenderTextureFBO::SaveTextureToFile(GLuint texturename, const std::string &filename) {
	int width = 0;
	int height = 0;
	glBindTexture(GL_TEXTURE_2D,texturename);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WIDTH,0,&width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,GL_TEXTURE_HEIGHT,0,&height);

#if 0
	//RenderTextureFBO fbo(width,height);
	//fbo.Init();
	//fbo.Use();
	Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glClearColor(0.0f,1.0f,0.0f,1.0f);
	g_ScreenQuad->Render(NULL);

	cv::Mat matImage;
	matImage.create(m_iwidth,m_iheight,CV_8UC3);
	std::vector<unsigned char> vecBuffer;
	vecBuffer.resize(m_iwidth * m_iheight * 4);
	glReadPixels(0,0,m_iwidth,m_iheight,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,&vecBuffer[0]);
#endif

    cv::Mat matImage;
    matImage.create(m_iwidth,m_iheight,CV_8UC3);
    std::vector<unsigned char> vecBuffer;
    vecBuffer.resize(m_iwidth * m_iheight * 4);
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_BYTE,&vecBuffer[0]);
	void CheckGLError();
	CheckGLError();
	int index = 0;
	for (int iY = 0; iY < matImage.rows; ++iY)
	{
		cv::Vec3b *vecBuf = matImage.ptr<cv::Vec3b>(matImage.rows - iY - 1);
		for (int iX = 0; iX < matImage.cols;++iX,++index)
		{
			vecBuf[iX][0] = vecBuffer[4 * index + 2];
			vecBuf[iX][1] = vecBuffer[4 * index + 1];
			vecBuf[iX][2] = vecBuffer[4 * index ];
		}
	}
	cv::imwrite(filename.c_str(),matImage);
	//fbo.UnUse();
	UnUse();
}
