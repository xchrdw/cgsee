#include <QString>
#include <QImage>

#include <core/material/imageqt.h>

#include <QGLWidget>

ImageQt::ImageQt(const QString & filePath)
    : Image(filePath)
{
    m_image = QGLWidget::convertToGLFormat(QImage(filePath));
}

ImageQt::~ImageQt() { }

bool ImageQt::texImage() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_image.width(), m_image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_image.bits());
    return !glError();
}