#pragma once

#include <core/core_api.h>

#include <memory>

class QString;
class QStringList;

class DataBlockRegistry;

class CORE_API AbstractLoader
{
public:
    AbstractLoader();
    virtual ~AbstractLoader() = 0;
    
    bool canLoad(const QString & extension) const;
    bool canSave(const QString & extension) const;

    virtual QStringList namedLoadableTypes() const;
    QStringList allLoadableTypes() const;

    virtual QStringList namedSaveableTypes() const;
    QStringList allSaveableTypes() const;

protected:
    virtual QStringList loadableExtensions() const;
    virtual QStringList saveableExtensions() const;
};
