#pragma once
#include "gl/glew.h"
class RenderTextureFBO
{
public:
	RenderTextureFBO(int width,int height);
public:
	bool Init();
private:
	int m_iwidth, m_iheight;
	GLuint fboID,depthID, renderTextureID;
public:
	bool CopyToTexture(GLuint texid);
	GLuint getTex()
	{
		return renderTextureID;
	}
public:
	void Use();
	void UnUse();
};
