#pragma once

#include <core/core_api.h>

class QString;

#include <core/abstractloader.h>

class DataBlockRegistry;
class Image;

class CORE_API AbstractImageLoader : public AbstractLoader
{
public:
    AbstractImageLoader();
    virtual ~AbstractImageLoader() = 0;

    virtual Image * importFromFile(const QString & filePath) const;
    virtual bool exportToFile(const Image & image,
        const QString & filePath, const QString & extension) const;
};
