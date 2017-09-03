//
// Created by zhaolei on 17-8-29.
//

#include "RenderDevice.h"

#include <glm/gtc/type_ptr.hpp>
using namespace glm;
RenderDevice::RenderDevice() {
    point2DVB = 0;
    point3DVB = 0;
    point2DVao = 0;
    point3DVao = 0;
}

void RenderDevice::Init() {
    // create the vertex buffers
    point2DShader.Build("shaders/point2d",{},{});
    point3DShader.Build("shaders/point3d",{},{"MVP"});
    initPointVBs();
}

void RenderDevice::Fini() {
    finiPointVBs();
    point2DShader.Fini();
    point3DShader.Fini();

}

void RenderDevice::Draw3DPoint(float x, float y, float z, unsigned long color) {
    glBindVertexArray(point3DVao);
    glEnable(GL_PROGRAM_POINT_SIZE);
    Point3DInfo info;
    info.x = x;
    info.y = y;
    info.z = z;
    info.size = 5.0f;
    info.color = color;
    glBindBuffer(GL_ARRAY_BUFFER,point3DVB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Point3DInfo),&info,GL_DYNAMIC_DRAW);
    point3DShader.Use();
    glm::mat4 matViewProj = matProj * matView;
    glUniformMatrix4fv(point3DShader["MVP"],1,GL_FALSE,glm::value_ptr(matViewProj));
    glDrawArrays(GL_POINTS,0,1);
    point3DShader.UnUse();
    glBindVertexArray(0);

}

void RenderDevice::Draw2DPoint(float x, float y, unsigned long color) {
    glBindVertexArray(point2DVao);
    glEnable(GL_PROGRAM_POINT_SIZE);
    Point2DInfo info;
    info.x = x;
    info.y = y;
    info.size = 5.0f;
    info.color = color;
    glBindBuffer(GL_ARRAY_BUFFER,point2DVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point2DInfo), &info, GL_DYNAMIC_DRAW);
    point2DShader.Use();
    glDrawArrays(GL_POINTS, 0, 1);
    point2DShader.UnUse();
    glBindVertexArray(0);
}

void RenderDevice::initPointVBs() {
    glGenBuffers(1, &point2DVB);
    glGenVertexArrays(1, &point2DVao);
    glBindVertexArray(point2DVao);
    glBindBuffer(GL_ARRAY_BUFFER, point2DVB);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point2DInfo), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point2DInfo), (const void *) offsetof(Point2DInfo, size));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Point2DInfo), (const void *) offsetof(Point2DInfo, color));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glGenBuffers(1, &point3DVB);

    glGenVertexArrays(1, &point3DVao);
    glBindVertexArray(point3DVao);
    glBindBuffer(GL_ARRAY_BUFFER, point3DVB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3DInfo), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point3DInfo), (const void *) offsetof(Point3DInfo, size));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Point3DInfo), (const void *) offsetof(Point3DInfo, color));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void RenderDevice::finiPointVBs() {
    if (point2DVB > 0) {
        glDeleteBuffers(1, &point2DVB);
        point2DVB = 0;
    }
    if (point3DVB > 0) {
        glDeleteBuffers(1, &point3DVB);
        point3DVB = 0;
    }
    if (point2DVao > 0) {
        glDeleteVertexArrays(1, &point2DVao);
        point2DVao = 0;
    }
    if (point3DVao > 0) {
        glDeleteVertexArrays(1, &point3DVao);
        point3DVao = 0;
    }
}

void RenderDevice::DrawTexture(GLuint textureid)
{
	glBindTexture(GL_TEXTURE_2D, textureid);
	GLint texwidth, texheight;
	texwidth = texheight = 0;
	// for 0 level mipmap
	glGetTextureLevelParameteriv(GL_TEXTURE_2D,0, GL_TEXTURE_WIDTH, &texwidth);
	glGetTextureLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texheight);

	// get viewport info
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int left = viewport[0];
	int top = viewport[1];
	int width = viewport[2];
	int height = viewport[3];
	int vpcenterx = left + width / 2;
	int vpcentery = top + height / 2;
	
	int drawleft = vpcenterx - texwidth / 2;
	int drawtop = vpcentery - texheight / 2;
	int drawright = vpcenterx + texwidth / 2;
	int drawbottom = vpcentery + texheight / 2;
	// get the four viewport pos

	// draw rect pos
}

glm::vec2 RenderDevice::GetViewportPosFromScreenPos(int x, int y)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int left = viewport[0];
	int top = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	// -1 .. 1
	float viewportx = ((x - left) / (float)width) * 2.0f - 1.0f;
	float viewporty = 1.0f - (y - top) / (float)height * 2.0f;
	return vec2(viewportx, viewporty);
}
