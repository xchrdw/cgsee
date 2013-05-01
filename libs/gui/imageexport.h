
#pragma once

#include <core/declspec.h>


class CGSEE_API ImageExport
{

public:
    ImageExport(QWidget * parent = nullptr);
    ImageExport(
        const QImage & image
    ,   QWidget * parent = nullptr);

    virtual ~ImageExport();

    const bool show() const;

    const bool save() const;
    const bool save(const QImage & image) const;

protected:
    QWidget * m_parent;

    mutable QString m_filePath;
    mutable QImage m_image;
};
