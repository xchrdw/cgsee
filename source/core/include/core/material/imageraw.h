#pragma once

#include <core/gpuquery.h>

#include <core/material/RawFile.h>

#include <core/material/image.h>

class QString;

class ImageRaw : public Image
{
public:
    ImageRaw(const QString & filePath);
    virtual ~ImageRaw();

    virtual bool isValid();

protected:
    virtual bool texImage();

protected:
    glraw::RawFile m_image;
};
