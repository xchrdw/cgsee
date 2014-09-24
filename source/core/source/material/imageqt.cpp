#include <QString>
#include <QImage>

#include <core/material/imageqt.h>

#include <QGLWidget>

ImageQt::ImageQt(const QString & filePath)
    : Image(filePath)
    , m_image(filePath)
{
    if (isValid())
        m_image = QGLWidget::convertToGLFormat(m_image);
}

ImageQt::~ImageQt() { }

bool ImageQt::isValid() const {
    return !m_image.isNull();
}

int ImageQt::width() const {
    return m_image.width();
}
int ImageQt::height() const {
    return m_image.height();
}

bool ImageQt::texImage() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    return !glError();
}