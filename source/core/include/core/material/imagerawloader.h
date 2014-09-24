#pragma once

#include <core/core_api.h>

class QString;

#include <core/abstractimageloader.h>

class DataBlockRegistry;
class Image;

class CORE_API ImageRawLoader : public AbstractImageLoader
{
public:
    ImageRawLoader();
    virtual ~ImageRawLoader();

    virtual QStringList namedLoadableTypes() const;

    virtual Image * importFromFile(const QString & filePath) const;

protected:
    virtual QStringList loadableExtensions() const;
};
