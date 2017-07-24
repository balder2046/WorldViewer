#pragma once
#include "gl/glew.h"
#include "gl/gl.h"
class CFrameBuffer
{
public:
	CFrameBuffer();
public:
	bool Build();
private:
	GLuint m_iFrameIndex;
};
