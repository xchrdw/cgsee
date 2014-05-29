#include <QString>

#include <core/material/imagerawloader.h>


#include <core/datacore/datablock.h>

#include <core/material/imageraw.h>

ImageRawLoader::ImageRawLoader()
    : AbstractImageLoader() {}

ImageRawLoader::~ImageRawLoader() {}

QStringList ImageRawLoader::namedLoadableTypes() const
{
    QStringList types;
    types
        << "All GlRaw Image Files (" + allLoadableTypes().join(" ") + ")";

    return types;
}

QStringList ImageRawLoader::loadableExtensions() const
{
    QStringList extensions;
    extensions
        << "glraw";

    return extensions;
}

Image * ImageRawLoader::importFromFile(const QString & filePath) const
{
    ImageRaw * image = new ImageRaw(filePath);
    if (!image->isValid()) {
        delete image;
        return nullptr;
    }
    else {
        return image;
    }
}