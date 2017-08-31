//
// Created by zhaolei on 17-8-29.
//

#include "RenderDevice.h"

RenderDevice::RenderDevice()
{
	point2DVB = 0;
	point3DVB = 0;
	point2DVao = 0;
	point3DVao = 0;
}

void RenderDevice::Init() {
		
}

void RenderDevice::Fini() {
	if (point2DVB > 0)
	{
		glDeleteBuffers(1, &point2DVB);
		point2DVB = 0;
	}
	if (point3DVB > 0)
	{
		glDeleteBuffers(1, &point3DVB);
		point3DVB = 0;
	}
	if (point2DVao > 0)
	{
		glDeleteVertexArrays(1, &point2DVao);
		point2DVao = 0;
	}
	if (point3DVao > 0)
	{
		glDeleteVertexArrays(1, &point3DVao);
		point3DVao = 0;
	}
}

void RenderDevice::Draw3DPoint(float x, float y, float z, unsigned long color) {

}

void RenderDevice::Draw2DPoint(float x, float y, unsigned long color) {

}
