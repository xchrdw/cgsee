#include <QString>

#include <core/abstractimageloader.h>


#include <core/datacore/datablock.h>

#include <core/material/image.h>

AbstractImageLoader::AbstractImageLoader()
    : AbstractLoader() {}

AbstractImageLoader::~AbstractImageLoader()
{
}

Image * AbstractImageLoader::importFromFile(const QString & filePath) const
{
    return nullptr;
}

bool AbstractImageLoader::exportToFile(const Image & image,
    const QString & filePath, const QString & extension) const
{
    return false;
}