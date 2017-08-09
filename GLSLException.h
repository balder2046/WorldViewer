//
// Created by zhaolei on 17-8-7.
//

#ifndef WORLDVIEWER_GLSLEXCEPTION_H
#define WORLDVIEWER_GLSLEXCEPTION_H
#include <exception>
#include <string>
class GLSLException : public std::exception
{
public:
    GLSLException(const char *msg)
    {
        message = msg;
    }
protected:
    GLSLException()
    {

    }
public:
    virtual const char *what() const noexcept
    {
        return message.c_str();
    }
protected:
    std::string message;
};

class GLSLAttribNotFoundException: public GLSLException
{
public:
    GLSLAttribNotFoundException(const char *attribname);

};
class GLSLUniformNotFoundException: public GLSLException
{
public:
    GLSLUniformNotFoundException(const char *uniformname);
};
class GLSLCompileErrorException: public GLSLException
{
public:
    GLSLCompileErrorException(const std::string &errorstring);
};

#endif //WORLDVIEWER_GLSLEXCEPTION_H
