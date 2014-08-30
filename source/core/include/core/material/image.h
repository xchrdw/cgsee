#pragma once

#include <core/core_api.h>

#include <core/gpuquery.h>

class Program;
class QString;

class CORE_API Image
{
public:
    Image(const QString & filePath);
    virtual ~Image() = 0;

    bool bind(const Program & program, const QString & uniformName, const GLubyte textureUnit);

    virtual bool isValid() = 0;

    virtual int width() = 0;
    virtual int height() = 0;

    virtual void setTexParameteri(GLenum pname, GLint param);
    virtual void setMagFilter(GLint param);
    virtual void setMinFilter(GLint param);

protected:
    virtual GLuint getTexture();
    virtual bool texImage() = 0;

protected:
    const QString & m_filePath;
    GLuint m_texture;
    bool m_textureGenerated;
};
