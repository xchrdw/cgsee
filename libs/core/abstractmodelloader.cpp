
#include "abstractpainter.h"
#include "group.h"
#include "abstractmodelloader.h"

bool AbstractModelLoader::canLoad(const QString & extension) const
{
    return loadableExtensions().contains(extension);
}

bool AbstractModelLoader::canSave(const QString & extension) const
{
    return saveableExtensions().contains(extension);
}

QString AbstractModelLoader::allLoadableTypes() const
{
    QStringList extensions = loadableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions.join(" ");
}

QString AbstractModelLoader::allSaveableTypes() const
{
    QStringList extensions = saveableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions.join(" ");
}

bool AbstractModelLoader::load(const QString & filename,
        AbstractPainter & painter) const
{
    Group * group = importFromFile(filename);
    if(!group)
        return false;

    painter.assignGroup(group);
    return true;
}

bool AbstractModelLoader::save(const AbstractPainter & painter, 
        const QString & filename, const QString & extension) const
{
    return exportToFile(painter.getGroup(), filename, extension);
}

bool AbstractModelLoader::save(const Group & group, 
        const QString & filename, const QString & extension) const
{
    return exportToFile(group, filename, extension);
}

