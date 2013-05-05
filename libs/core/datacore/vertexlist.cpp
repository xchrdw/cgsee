#include "vertexlist.h"

AttributeStorage::AttributeStorage(const t_AttrMap &attrMap)
{
    unsigned int memoryNeeded = 0;

    for (const t_AttrDesc &attr: attrMap)
        memoryNeeded = qMax(memoryNeeded, attr.size + attr.location);

    m_storage = new unsigned char[memoryNeeded];
    assert(m_storage != nullptr);

    for (const t_AttrDesc &attr: attrMap)
    {
        attr.factory->construct(m_storage + attr.location);
    }
}

AttributeStorage::~AttributeStorage()
{
    // TODO
}

