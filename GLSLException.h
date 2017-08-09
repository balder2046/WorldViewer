//
// Created by zhaolei on 17-8-7.
//

#ifndef WORLDVIEWER_GLSLEXCEPTION_H
#define WORLDVIEWER_GLSLEXCEPTION_H
#include <exception>
#include <string>
class GLSLException : public std::exception
{
protected:
    GLSLException(const char *msg)
    {
        message = msg;
    }
public:
    virtual const char *what() const
    {
        return message.c_str();
    }
private:
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

#endif //WORLDVIEWER_GLSLEXCEPTION_H
