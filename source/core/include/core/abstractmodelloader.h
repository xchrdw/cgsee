#pragma once

#include <core/core_api.h>

#include <memory>

#include <QString>
#include <QStringList>

class DataBlockRegistry;
class Group;

class CORE_API AbstractModelLoader
{
public:
    AbstractModelLoader(std::shared_ptr<DataBlockRegistry> registry = nullptr);
    virtual ~AbstractModelLoader();
    
    bool canLoad(const QString & extension) const;
    bool canSave(const QString & extension) const;

    virtual QStringList namedLoadableTypes() const;
    QStringList allLoadableTypes() const;

    virtual QStringList namedSaveableTypes() const;
    QStringList allSaveableTypes() const;

    virtual Group * importFromFile(const QString & filePath) const;
    virtual bool exportToFile(const Group & group,
        const QString & filePath, const QString & extension) const;
protected:
    virtual QStringList loadableExtensions() const;
    virtual QStringList saveableExtensions() const;

protected:
    std::shared_ptr<DataBlockRegistry> m_registry;
};
