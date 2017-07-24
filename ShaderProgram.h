//
// Created by ad on 17-7-23.
//

#ifndef WORLDVIEWER_SHADERPROGRAM_H
#define WORLDVIEWER_SHADERPROGRAM_H
#include <string>
#include "GL/glew.h"
class ShaderProgram {
public:
    void SetVertexShaderFile(const std::string &vertexfilename);
    void SetFragmentShaderFile(const std::string &fragmentfilename);
    void Build();
    void Freee();
    void UseProgram();
private:
    GLuint m_program;
    std::string m_strvertexfilename;
    std::string m_strfragmentfilename;

};


#endif //WORLDVIEWER_SHADERPROGRAM_H
