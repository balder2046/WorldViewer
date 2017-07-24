#include "stdafx.h"
#include "gl/glew.h"
#include "RenderTextureFBO.h"
#include "ScreenSizeQuad.h"

RenderTextureFBO::RenderTextureFBO(int width, int height)
{
	m_iwidth = width;
	m_iheight = height;
	fboID = 0;
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


void RenderTextureFBO::Use()
{
	//enable FBO 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
	//render to colour attachment 0
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
void RenderTextureFBO::UnUse()
{
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
	return true;
}