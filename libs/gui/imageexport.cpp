
#include <QImageReader>

#include "ui_imageexport.h"
#include "imageexport.h"

#include <core/abstractpainter.h>


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

    // TODO: restore with QSettings
    // - last file path
    // - all other settings (alpha, size, aspect, units displayed in ...)

}

ImageExport::~ImageExport()
{
}
//
//const bool ImageExport::show()        // TODO: do this on OK event... 
//{
//    //m_filewidget = new FileWidget(this, "FileDialog");
//
//    //static QString filter;
//    //if(filter.isEmpty())
//    //{
//    //    const QList<QByteArray> formats = QImageReader::supportedImageFormats();
//    //    QStringList extensions;
//
//    //    foreach(const QByteArray & format, formats)
//    //        extensions << QString("*." + QString::fromLocal8Bit(format).toLower());
//
//    //    filter = QObject::tr("Images (%1)\nAll Files (*.*)").arg(extensions.join("; "));
//    //}
//
//    //m_filewidget->setNameFilter(filter);
//
//
//
//
//    //if (this->exec() == QDialog::Accepted) {
//    //    m_filePath = m_filewidget->selectedFiles().value(0);
//    //    return !m_filePath.isEmpty();
//    //}
//
//    return false;
//}

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

const bool ImageExport::aspect() const
{
    return m_aspect;
}

const bool ImageExport::alpha() const
{
    return m_alpha;
}

const QSize & ImageExport::size() const
{
    return m_size;
}

const QString & ImageExport::filePath() const
{
    return m_filePath;
}

const bool ImageExport::save(
    AbstractPainter & painter
,   QWidget * parent)
{
    ImageExport ie(parent);

    if(!ie.exec())
        return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    QImage image(painter.capture(ie.size(), ie.aspect(), ie.alpha()));
    const bool result = ie.save(image);

    QApplication::restoreOverrideCursor();

    if(!result)
        qWarning("Image export to \"%s\" failed", qPrintable(ie.filePath()));

    return result ;
}