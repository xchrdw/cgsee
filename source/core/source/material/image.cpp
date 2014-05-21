#include <QString>

#include <core/material/image.h>

#include <core/program.h>

Image::Image(const QString & filePath)
    : m_filePath(filePath)
{ }

Image::~Image() {}

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