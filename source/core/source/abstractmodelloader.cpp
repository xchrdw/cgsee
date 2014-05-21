#include <QString>

#include <core/abstractmodelloader.h>

#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>

AbstractModelLoader::AbstractModelLoader(std::shared_ptr<DataBlockRegistry> registry)
    : AbstractLoader()
    , m_registry(registry)
{
    if (m_registry == nullptr)
        m_registry = std::make_shared<DataBlockRegistry>();
}

AbstractModelLoader::~AbstractModelLoader()
{
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