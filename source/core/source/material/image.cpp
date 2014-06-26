#include <QString>

#include <core/material/image.h>

#include <core/program.h>

Image::Image(const QString & filePath)
    : m_filePath(filePath)
    , m_textureGenerated(false)
{ }

Image::~Image() {
    if (m_textureGenerated)
        glDeleteTextures(1, &m_texture);
}

bool Image::bind(const Program & program, const QString & uniformName, const GLubyte textureUnit) {
    if (textureUnit >= 32)
        return false;

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    if (glError())
        return false;

    glBindTexture(GL_TEXTURE_2D, getTexture());
    if (glError())
        return false;

    program.setUniform(uniformName, textureUnit);
    return true;
}

GLuint Image::getTexture() {
    if (m_textureGenerated)
        return m_texture;

    glGenTextures(1, &m_texture);
    glError();
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glError();

    if (!texImage())
        return 0;
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glError();

    return m_texture;
}