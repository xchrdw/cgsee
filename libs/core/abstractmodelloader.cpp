
#include "abstractpainter.h"
#include "group.h"
#include "abstractmodelloader.h"

AbstractModelLoader::AbstractModelLoader()
{
}

AbstractModelLoader::~AbstractModelLoader()
{
}

bool AbstractModelLoader::canLoad(const QString & extension) const
{
    return loadableExtensions().contains(extension);
}

bool AbstractModelLoader::canSave(const QString & extension) const
{
    return saveableExtensions().contains(extension);
}

QStringList AbstractModelLoader::loadableTypes() const
{
    return QStringList();
}

QString AbstractModelLoader::allLoadableTypes() const
{
    QStringList extensions = loadableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions.join(" ");
}

QStringList AbstractModelLoader::saveableTypes() const
{
    return QStringList();
}

QString AbstractModelLoader::allSaveableTypes() const
{
    QStringList extensions = saveableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions.join(" ");
}

Group * AbstractModelLoader::importFromFile(const QString & filePath) const
{
    return nullptr;
}

bool AbstractModelLoader::exportToFile(const Group & group,
    const QString & filePath, const QString & extension) const
{
    return false;
}

QStringList AbstractModelLoader::loadableExtensions() const
{
    return QStringList();
}

QStringList AbstractModelLoader::saveableExtensions() const
{
    return QStringList();
}

