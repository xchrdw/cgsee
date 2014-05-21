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

protected:
    virtual GLuint getTexture();
    virtual bool texImage() = 0;

protected:
    const QString & m_filePath;
    GLuint m_texture;
    bool m_textureGenerated;
};
