#include <QString>
#include <QStringList>

#include <core/abstractloader.h>

#include <core/datacore/datablock.h>

AbstractLoader::AbstractLoader()
{
}

AbstractLoader::~AbstractLoader()
{
}

bool AbstractLoader::canLoad(const QString & extension) const
{
    return loadableExtensions().contains(extension);
}

bool AbstractLoader::canSave(const QString & extension) const
{
    return saveableExtensions().contains(extension);
}

QStringList AbstractLoader::namedLoadableTypes() const
{
    QString allTypes = "All Supported Files (" + allLoadableTypes().join(" ") + ")";
    return QStringList(allTypes);
}

QStringList AbstractLoader::allLoadableTypes() const
{
    QStringList extensions = loadableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions;
}

QStringList AbstractLoader::namedSaveableTypes() const
{
    QString allTypes = "All Supported Files (" + allSaveableTypes().join(" ") + ")";
    return QStringList(allTypes);
}

QStringList AbstractLoader::allSaveableTypes() const
{
    QStringList extensions = saveableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions;
}

QStringList AbstractLoader::loadableExtensions() const
{
    return QStringList();
}

QStringList AbstractLoader::saveableExtensions() const
{
    return QStringList();
}

