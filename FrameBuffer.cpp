#include "FrameBuffer.h"
#include "GL/glew.h"
CFrameBuffer::CFrameBuffer()
{
	m_iFrameIndex = 0;
}

bool CFrameBuffer::Build()
{
	glGenFramebuffers(1, &m_iFrameIndex);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_iFrameIndex);
	return true;
}
