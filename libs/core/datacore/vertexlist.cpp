#include <assert.h>

#include "vertexlist.h"

namespace
{
    struct TypeSpec
    {
        unsigned int size;
        AbstractInPlaceObjectFactory *factory;
        const type_info &typeInfo;
    };

    QMap<QString, TypeSpec> g_registeredTypes;
}

AbstractInPlaceObjectFactory::~AbstractInPlaceObjectFactory()
{

}

QObjectFactory::QObjectFactory(const char * qTypeName)
{
    m_typeId = QMetaType::type(qTypeName);
    assert(m_typeId != 0);
}

void QObjectFactory::construct(void * place) const
{
    assert(m_typeId != 0);
    QMetaType::construct(m_typeId, place, nullptr);
}

void QObjectFactory::destruct(void * place) const
{
    assert(m_typeId != 0);
    QMetaType::destroy(m_typeId, place);
}

AttributeSpec::AttributeSpec(const QString &name, const QString &type):
    attrName(name)
,   attrType(type)
{
    
}

AttributeStorage::AttributeStorage(const t_AttrMap &attrMap):
    m_destroyed(false)
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
    if (!m_destroyed)
        //something bad has happened
        qWarning(QString("Destroying Attribute storage with objects in it intact. Potential leak. In %1, line %2").arg(__FILE__).arg(__LINE__).toLocal8Bit());
}

void AttributeStorage::runDestructors(const t_AttrMap &attrMap)
{
    if (m_destroyed)
        return;

    for (const t_AttrDesc & attr: attrMap)
        attr.factory->destruct(m_storage + attr.location);

    m_destroyed = true;
}

VertexList::VertexList():
    m_initialized(false)
{
    
}

VertexList::~VertexList()
{
    //TODO
}

void VertexList::initialize(const QList<AttributeSpec> &attrTypes)
{
    //TODO

    m_initialized = true;
}

