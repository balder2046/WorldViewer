//
// Created by zhaolei on 17-8-29.
//

#ifndef WORLDVIEWER_RENDERDEVICE_H
#define WORLDVIEWER_RENDERDEVICE_H
#include <glm/glm.hpp>
#include "GLSLShader.h"

class RenderDevice {
public:
	RenderDevice();
    void Init();
    void Fini();
    void Draw3DPoint(float x,float y,float z,unsigned long color);
    void Draw2DPoint(float x, float y, unsigned long color);
	void DrawTexture(GLuint textureid);
public:
	glm::vec2 GetViewportPosFromScreenPos(int x,int y);
protected:
	void initPointVBs();
	void finiPointVBs();
	struct Point3DInfo
	{
		float x,y,z;
		float size;
		unsigned long color;
	};
	struct Point2DInfo
	{
		float x,y;
		float size;
		unsigned long color;
	};
private:
	GLuint point2DVB;
	GLuint point3DVB;
	GLuint point2DVao;
	GLuint point3DVao;
	GLuint textureVB;
	GLuint textureVao;
	GLuint textureIB;

private:
	GLSLShader point2DShader;
	GLSLShader point3DShader;
public:
	void SetViewProj(const glm::mat4 &view,const glm::mat4 &proj)
	{
		matView = view;
		matProj = proj;
	}
private:
	glm::mat4 matView;
	glm::mat4 matProj;

};


#endif //WORLDVIEWER_RENDERDEVICE_H
