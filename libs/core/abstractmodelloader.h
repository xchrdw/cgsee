
#pragma once

#include <QString>
#include <QStringList>
#include <core/declspec.h>

class Group;

class CGSEE_API AbstractModelLoader
{
public:
    AbstractModelLoader();
    virtual ~AbstractModelLoader();
    
    bool canLoad(const QString & extension) const;
    bool canSave(const QString & extension) const;

    virtual QStringList loadableTypes() const;
    QString allLoadableTypes() const;

    virtual QStringList saveableTypes() const;
    QString allSaveableTypes() const;

    virtual Group * importFromFile(const QString & filePath) const;
    virtual bool exportToFile(const Group & group,
        const QString & filePath, const QString & extension) const;
protected:
    virtual QStringList loadableExtensions() const;
    virtual QStringList saveableExtensions() const;
};

