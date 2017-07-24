//
// Created by ad on 17-7-23.
//

#include "ShaderProgram.h"
#include <fstream>
#include "vector"
void ShaderProgram::SetVertexShaderFile(const std::string &vertexfilename) {
    m_strvertexfilename = vertexfilename;

}

void ShaderProgram::SetFragmentShaderFile(const std::string &fragmentfilename) {
    m_strfragmentfilename = fragmentfilename;

}

void ShaderProgram::Build() {
    std::vector<char> buf;
    buf.resize(1024 * 1024);
    std::fstream vertfile;
    vertfile.open(m_strvertexfilename,std::ios_base::in | std::ios_base::binary);
    vertfile.read(&buf[0],1024 * 1024);

}
