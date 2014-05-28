#include <QString>
#include <QImageReader>

#include <core/material/imageqtloader.h>


#include <core/datacore/datablock.h>

#include <core/material/imageqt.h>

ImageQtLoader::ImageQtLoader()
    : AbstractImageLoader() {}

ImageQtLoader::~ImageQtLoader() {}

QStringList ImageQtLoader::namedLoadableTypes() const
{
    QMap<QString, QString> typenames;
    typenames.insert("bmp", "Windows Bitmap");
    typenames.insert("dds", "DirectDraw Surface");
    typenames.insert("gif", "Graphic Interchange Format");
    typenames.insert("icns", "Apple Icon Image");
    typenames.insert("ico", "Windows Icon Image");
    typenames.insert("jp2", "JPEG 2000");
    typenames.insert("jpg", "Joint Photographic Experts Group");
    typenames.insert("jpeg", "Joint Photographic Experts Group");
    typenames.insert("mng", "Multiple-image Network Graphics");
    typenames.insert("png", "Portable Network Graphics");
    typenames.insert("pbm", "Portable Bitmap");
    typenames.insert("pgm", "Portable Graymap");
    typenames.insert("ppm", "Portable Pixmap");
    typenames.insert("svg", "Scalable Vector Graphics");
    typenames.insert("svgz", "Scalable Vector Graphics");
    typenames.insert("tga", "Truevision Graphics Adapter");
    typenames.insert("tif", "Tagged Image File Format");
    typenames.insert("tiff", "Tagged Image File Format");
    typenames.insert("wbmp", " Wireless Bitmap");
    typenames.insert("webp", "WebP");
    typenames.insert("xbm", "X11 Bitmap");
    typenames.insert("xpm", "X11 Bitmap");

    QStringList types;
    types
        << "All Qt Image Files (" + allLoadableTypes().join(" ") + ")";
    QStringList loadable = loadableExtensions();
    for (QString extension : loadable) {
        if (typenames.contains(extension)) {
            types << typenames[extension].append("(*.").append(extension).append(")");
        }
        else {
            types << QString("(*.").append(extension).append(")");
        }
    }

    return types;
}

QStringList ImageQtLoader::loadableExtensions() const
{
    QList<QByteArray> sif = QImageReader::supportedImageFormats();
    QStringList extensions = QStringList();
    for (QByteArray ba : sif) {
        extensions << ba.constData();
    }

    return extensions;
}

Image * ImageQtLoader::importFromFile(const QString & filePath) const
{
    ImageQt * image = new ImageQt(filePath);
    if (!image->isValid()) {
        delete image;
        return nullptr;
    }
    else {
        return image;
    }
}