#pragma once

#include <core/gpuquery.h>

#include <QImage>

#include <core/material/image.h>

class QString;

class ImageQt : public Image
{
public:
    ImageQt(const QString & filePath);
    virtual ~ImageQt();

    virtual bool isValid();

protected:
    virtual bool texImage();

protected:
    QImage m_image;
};