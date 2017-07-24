#pragma once
#include "GL/glew.h"
class CFrameBuffer
{
public:
	CFrameBuffer();
public:
	bool Build();
private:
	GLuint m_iFrameIndex;
};
