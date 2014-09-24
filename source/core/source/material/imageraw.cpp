#include <QString>
#include <core/material/RawFile.h>

#include <core/material/imageraw.h>

#include <QGLWidget>

ImageRaw::ImageRaw(const QString & filePath)
    : Image(filePath)
    , m_image(filePath.toStdString())
{ }

ImageRaw::~ImageRaw() { }

bool ImageRaw::isValid() const {
    return m_image.isValid() && m_image.hasIntProperty("width") && m_image.hasIntProperty("height");
}

int ImageRaw::width() const {
    return m_image.intProperty("width");
}
int ImageRaw::height() const {
    return m_image.intProperty("height");
}

bool ImageRaw::texImage() {
    int w = width();
    int h = height();

    if (m_image.hasIntProperty("format"))
    {
        GLenum format = static_cast<GLenum>(m_image.intProperty("format"));
        GLenum type = static_cast<GLenum>(m_image.intProperty("type"));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, type, m_image.data());
    }
    else
    {
        GLenum compressedFormat = static_cast<GLenum>(m_image.intProperty("compressedFormat"));
        GLenum size = m_image.intProperty("size");

        glCompressedTexImage2D(GL_TEXTURE_2D, 0, compressedFormat, w, h, 0, size, m_image.data());
    }

    return !glError();
}