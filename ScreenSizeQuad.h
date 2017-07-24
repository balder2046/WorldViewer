#pragma once
#include "RenderableObject.h"
class CScreenSizeQuad :
	public RenderableObject
{
public:
	CScreenSizeQuad(void);
	virtual ~CScreenSizeQuad(void);
	int GetTotalVertices() {
		return 4;
	}

	int GetTotalIndices() {
		return 6;
	}

	GLenum GetPrimitiveType() {
		return GL_TRIANGLES;
	}
	void FillVertexBuffer(GLfloat* pBuffer);
	void FillIndexBuffer(GLuint* pBuffer);

	void Render(const float* MVP);
};

