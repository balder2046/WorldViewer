//
// Created by zhaolei on 17-8-7.
//

#include "GLSLException.h"

GLSLAttribNotFoundException::GLSLAttribNotFoundException(const char *attribname)
{
    char szBuf[256];
    snprintf(szBuf,256,"attrib %s not found !",attribname);
    message = szBuf;
}

GLSLUniformNotFoundException::GLSLUniformNotFoundException(const char *uniformname) {
    char szBuf[256];
    snprintf(szBuf,256,"uniform %s not found !",uniformname);
    message = szBuf;
}

GLSLCompileErrorException::GLSLCompileErrorException(const std::string &errorstring) {
    message = errorstring;
}
