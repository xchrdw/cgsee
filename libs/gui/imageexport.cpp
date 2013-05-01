
#include <QImageReader>
#include <QFileDialog>
#include <QSettings>

#include "imageexport.h"


ImageExport::ImageExport(QWidget * parent)
:   m_parent(parent)
{
}

ImageExport::ImageExport(
    const QImage & image
,   QWidget * parent)
:   m_parent(parent)
,   m_image(image)
{
}

ImageExport::~ImageExport()
{
}

const bool ImageExport::show() const
{
    static QString filter;    
    if(filter.isEmpty())
    {
        const QList<QByteArray> formats = QImageReader::supportedImageFormats();
        QStringList extensions;

        foreach(const QByteArray & format, formats)
            extensions << QString("*." + QString::fromLocal8Bit(format).toLower());

        filter = QObject::tr("Images (%1)\nAll Files (*.*)").arg(extensions.join("; "));
    }

    m_filePath = QFileDialog::getSaveFileName(m_parent, QString("Export Image"), QString(), filter);
    return !m_filePath.isEmpty();    
}

const bool ImageExport::save(
    const QImage & image) const
{
    m_image = image;
    return save();
}

const bool ImageExport::save() const
{
    if(m_filePath.isEmpty())
        return false;

    if(m_image.isNull())
    {
        qWarning("No image for export.");
        return false;
    }

    return m_image.save(m_filePath);
}