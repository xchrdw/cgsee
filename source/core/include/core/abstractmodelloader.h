#pragma once

#include <core/core_api.h>

#include <memory>

class QString;

#include <core/abstractloader.h>

class DataBlockRegistry;
class Group;

class CORE_API AbstractModelLoader : public AbstractLoader
{
public:
    AbstractModelLoader(std::shared_ptr<DataBlockRegistry> registry = nullptr);
    virtual ~AbstractModelLoader() = 0;

    virtual Group * importFromFile(const QString & filePath) const;
    virtual bool exportToFile(const Group & group,
        const QString & filePath, const QString & extension) const;

protected:
    std::shared_ptr<DataBlockRegistry> m_registry;
};
