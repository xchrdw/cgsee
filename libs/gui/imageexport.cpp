
#include <QImageReader>

#include "ui_imageexport.h"
#include "imageexport.h"
#include "filewidget.h"


ImageExport::ImageExport(QWidget * parent)
:   m_parent(parent)
,   m_ui(new Ui_ImageExport)
{
    initialize();
}

ImageExport::ImageExport(
    const QImage & image
,   QWidget * parent)
:   m_parent(parent)
,   m_image(image)
,   m_ui(new Ui_ImageExport)
{
    initialize();
}

void ImageExport::initialize()
{
    m_ui->setupUi(this);

    // embedded file dialog as widget
    m_filewidget = new FileWidget(this, "FileDialog");

    static QString filter;
    if(filter.isEmpty())
    {
        const QList<QByteArray> formats = QImageReader::supportedImageFormats();
        QStringList extensions;

        foreach(const QByteArray & format, formats)
            extensions << QString("*." + QString::fromLocal8Bit(format).toLower());

        filter = QObject::tr("Images (%1)\nAll Files (*.*)").arg(extensions.join("; "));
    }

    m_filewidget->setNameFilter(filter);

    m_ui->filedialoglayout->addWidget(m_filewidget);

    // options

    // ...
}

ImageExport::~ImageExport()
{
    //
}

const bool ImageExport::show()
{
    if (this->exec() == QDialog::Accepted) {
        m_filePath = m_filewidget->selectedFiles().value(0);
        return !m_filePath.isEmpty();
    }

    return false;
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
