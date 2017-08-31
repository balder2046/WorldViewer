//
// Created by zhaolei on 17-8-29.
//

#ifndef WORLDVIEWER_RENDERDEVICE_H
#define WORLDVIEWER_RENDERDEVICE_H

#include "GLSLShader.h"

class RenderDevice {
public:
	RenderDevice();
    void Init();
    void Fini();
    void Draw3DPoint(float x,float y,float z,unsigned long color);
    void Draw2DPoint(float x, float y, unsigned long color);
private:
	GLuint point2DVB;
	GLuint point3DVB;
	GLuint point2DVao;
	GLuint point3DVao;
};


#endif //WORLDVIEWER_RENDERDEVICE_H
