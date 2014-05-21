#include <QString>

#include <core/material/imageqtloader.h>


#include <core/datacore/datablock.h>

#include <core/material/imageqt.h>

ImageQtLoader::ImageQtLoader(std::shared_ptr<DataBlockRegistry> registry)
    : AbstractImageLoader(registry) {}

ImageQtLoader::~ImageQtLoader()
{
}

QStringList ImageQtLoader::namedLoadableTypes() const
{
    QStringList types = AbstractLoader::namedLoadableTypes();
    types
        << "Windows Bitmap (*.bmp)"
        << "Graphic Interchange Format (*.gif)"
        << "Joint Photographic Experts Group (*.jpg)"
        << "Joint Photographic Experts Group (*.jpeg)"
        << "Portable Network Graphics (*.png)"
        << "Portable Bitmap (*.pbm)"
        << "Portable Graymap (*.pgm)"
        << "Portable Pixmap (*.ppm)"
        << "X11 Bitmap (*.xbm)"
        << "X11 Bitmap (*.xpm)";

    return types;
}

QStringList ImageQtLoader::loadableExtensions() const
{
    QStringList extensions = QStringList();
    extensions
        << "bmp"
        << "gif"
        << "jpg"
        << "jpeg"
        << "png"
        << "pbm"
        << "pgm"
        << "ppm"
        << "xbm"
        << "xpm";

    return extensions;
}

Image * ImageQtLoader::importFromFile(const QString & filePath) const
{
    return new ImageQt(filePath);
}