#pragma once

#include <core/core_api.h>

#include <memory>

class QString;

#include <core/abstractimageloader.h>

class DataBlockRegistry;
class Image;

class CORE_API ImageQtLoader : public AbstractImageLoader
{
public:
    ImageQtLoader(std::shared_ptr<DataBlockRegistry> registry = nullptr);
    virtual ~ImageQtLoader() = 0;

    virtual QStringList namedLoadableTypes() const;

    virtual Image * importFromFile(const QString & filePath) const;

protected:
    virtual QStringList loadableExtensions() const;
};
