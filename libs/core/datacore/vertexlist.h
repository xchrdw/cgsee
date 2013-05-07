#ifndef VERTEXLIST_H
#define VERTEXLIST_H
#pragma once

#include <typeinfo>
#include <memory>

#include "abstractdata.h"

struct AbstractInPlaceObjectFactory
{
    virtual ~AbstractInPlaceObjectFactory();
    virtual void construct(void * place) const=0;
    //virtual void destruct(void * place) const=0;
};

template <typename T>
struct InPlaceObjectFactory : public AbstractInPlaceObjectFactory
    , public std::enable_shared_from_this<InPlaceObjectFactory<T> >
{
    void construct(void * place) const
    {
        new(place) T;
    }
    //void destruct(void * place) const
    //{
    //    static_cast<T*>(place)->~T();
    //}
};

class QObjectFactory : public AbstractInPlaceObjectFactory
{
public:
    QObjectFactory(const char * qTypeName);

    void construct(void * place) const;
    //void destruct(void * place) const;

protected:
    int m_typeId;
};

// describes layout of vertex attributes in a storage.
typedef struct AttributeDescriptor
{
    unsigned int location;
    unsigned int size;
    QString typeName;
    std::shared_ptr<AbstractInPlaceObjectFactory> factory;
    type_info const* typeInfo;
} t_AttrDesc;

struct AttributeSpec
{
    QString attrName;
    QString attrType;

    AttributeSpec(const QString &name, const QString &type);
};

typedef QHash<QString, t_AttrDesc> t_AttrMap;

// A vertex (and everything really) can be described by its attributes.
// This class provides a storage for them, but it doesn't memorize the right layout.
// Which is not needed anyway, as most of the times many vertecies have same attribute types.
class AttributeStorage final
{
public:
    typedef unsigned char * t_StorageType;

    AttributeStorage();
    AttributeStorage(AttributeStorage &&rhs);
    // Initializes attributeStorage and creates in-place default constructed
    // data objects, which hold 
    explicit AttributeStorage(const t_AttrMap &attrMap);

    // copy constructor and assignment.
    AttributeStorage(const AttributeStorage&);
    AttributeStorage const& operator =(const AttributeStorage&);

    ~AttributeStorage();

    void initialize(const t_AttrMap &attrMap);

    // prepares storage for deletion.
    // invalidates its content
    void runDestructors(const t_AttrMap &attrMap);

    // Returns a pointer to attribute data object(eg. glm::vec3), 
    // checks for the right type(with typeid) and returns nullptr
    // when something is wrong
    template <class RetType>
    RetType* getData(const t_AttrDesc &loc); 
protected:
    t_StorageType m_storage;
    unsigned int m_storageSize;
    bool m_destroyed;

    mutable unsigned int * m_useCount;

    void copyStorage();
};

template <class RetType>
RetType * AttributeStorage::getData(const t_AttrDesc & loc)
{
    if (m_destroyed)
        return nullptr;

    copyStorage();
    AttributeStorage::t_StorageType resultPtr = m_storage + loc.location;
    if (typeid(resultPtr) != loc.typeInfo)
        return nullptr;

    return static_cast<RetType*> (resultPtr);
}

class CGSEE_API VertexList: public DataBlock
{
    Q_OBJECT
public:
    VertexList();
    ~VertexList();

    void initialize(const QList<AttributeSpec> &attrTypes);

    template <class RetType>
    RetType * getVertexAttribute(int index, const QString &attrName);

protected:
    QVector<AttributeStorage> m_vertices;
    t_AttrMap m_attrLayout; 
    
    bool m_initialized;
};

template <class RetType>
RetType * VertexList::getVertexAttribute(int index, const QString &attrName)
{
    if (!m_initialized)
        return nullptr;

    if (m_vertices.size() <= index)
    {
        createNewVertices(index - m_vertices.size() + 1);
    }

    if (m_attrLayout.contains(attrName) == false)
        return nullptr;

    return m_vertices.at(index).getData(m_attrLayout[attrName]);
}

#endif