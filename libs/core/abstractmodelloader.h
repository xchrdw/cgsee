
#pragma once

#include <QString>
#include <QStringList>
#include <core/declspec.h>

class AbstractPainter;
class Group;

class CGSEE_API AbstractModelLoader
{
public:
    bool canLoad(const QString & extension) const;
    bool canSave(const QString & extension) const;

    virtual QStringList loadableTypes() const =0;
    QString allLoadableTypes() const;

    virtual QStringList saveableTypes() const =0;
    QString allSaveableTypes() const;

    bool load(const QString & filename, AbstractPainter & painter) const;
    bool save(const AbstractPainter & painter, 
        const QString & filename, const QString & extension) const;
    bool save(const Group & group, 
        const QString & filename, const QString & extension) const;
protected:
    virtual Group * importFromFile(const QString & filename) const =0;
    virtual bool exportToFile(const Group & group, 
        const QString & filename, const QString & extension) const =0;
    virtual QStringList loadableExtensions() const =0;
    virtual QStringList saveableExtensions() const =0;
};

