#include <assert.h>

#include "vertexlist.h"

namespace
{
    struct TypeSpec
    {
        unsigned int size;
        std::shared_ptr<AbstractInPlaceObjectFactory> factory;
        const type_info *typeInfo;
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

AttributeStorage::AttributeStorage():
    m_storage(nullptr)
,   m_destroyed(false)
{
}

AttributeStorage::AttributeStorage(AttributeStorage &&rhs):
    m_storage(rhs.m_storage)
,   m_destroyed(rhs.m_destroyed)
{
    rhs.m_storage = nullptr;
    rhs.m_destroyed = true;
}

AttributeStorage::AttributeStorage(const t_AttrMap &attrMap):
    m_storage(nullptr)
,   m_destroyed(false)
{
    initialize(attrMap);
}

AttributeStorage::~AttributeStorage()
{
    if (m_storage && !m_destroyed)
        //something bad has happened
        qWarning(QObject::tr("Destroying Attribute storage with objects in it intact. Potential leak. In %1, line %2").arg(__FILE__).arg(__LINE__).toLocal8Bit());
    delete [] m_storage;
}

void AttributeStorage::initialize(const t_AttrMap &attrMap)
{
    assert(m_storage == nullptr);
    delete [] m_storage;
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
    if (m_initialized)
    {
        for (AttributeStorage &stor: m_vertices)
            stor.runDestructors(m_attrLayout);
    }
}

void VertexList::initialize(const QList<AttributeSpec> &attrTypes)
{
    if (m_initialized)
        return;

    m_attrLayout.clear();

    unsigned int currentLoc = 0;
    for (AttributeSpec const & attrSpec : attrTypes)
    {
        t_AttrDesc newAttribute;
        newAttribute.location = currentLoc;
        
        // Looking if Qt knows about this type
        int attrTypeId = QMetaType::type(attrSpec.attrType.toUtf8());
        if (0 == attrTypeId)
        {
            // Apparently it doesn't
            if (false == g_registeredTypes.contains(attrSpec.attrType))
            {
                qWarning(tr("Cannot find type info for type %1, on line %2, in file %3. Ignoring.")
                    .arg(attrSpec.attrType)
                    .arg(__LINE__)
                    .arg(__FILE__)
                    .toLocal8Bit());
                continue;
            }
            const TypeSpec &typeSpec = g_registeredTypes[attrSpec.attrType];
            newAttribute.size = typeSpec.size;
            newAttribute.factory = typeSpec.factory;
            newAttribute.typeInfo = typeSpec.typeInfo;
        }
        else
        {
            newAttribute.size = QMetaType::sizeOf(attrTypeId);
            newAttribute.factory = decltype(t_AttrDesc::factory)
                (new QObjectFactory(attrSpec.attrType.toUtf8()));
            newAttribute.typeInfo = &typeid(QMetaType);
        }

        newAttribute.typeName = attrSpec.attrType;
        m_attrLayout.insert(attrSpec.attrName, newAttribute);
    }
    m_initialized = true;
}

