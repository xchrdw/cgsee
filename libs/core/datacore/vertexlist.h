#ifndef VERTEXLIST_H
#define VERTEXLIST_H
#pragma once

#include <glm/glm.hpp>
#include <typeinfo>
#include <functional>

#include "abstractdata.h"

struct AbstractInPlaceObjectFactory
{
    virtual void construct(void * place) const=0;
    virtual void destruct(void * place) const=0;
};

template <typename T>
struct InPlaceObjectFactory
{
    void construct(void * place) const
    {
        new(place) T;
    }
    void destruct(void * place) const
    {
        static_cast<T*>(place)->~T();
    }
};

// describes layout of vertex attributes in a storage.
typedef struct AttributeDescriptor
{
    unsigned int location;
    unsigned int size;
    QString typeName;
    AbstractInPlaceObjectFactory *factory;
    const type_info & typeInfo;
} t_AttrDesc;


typedef QHash<QString, t_AttrDesc> t_AttrMap;

// A vertex (and everything really) can be described by its attributes.
// This class provides a storage for them, but it doesn't memorize the right layout.
// Which is not needed anyway, as most of the times many vertecies have same attribute types.
class AttributeStorage final
{
public:
    typedef unsigned char * t_StorageType;

    // Initializes attributeStorage and creates in-place default constructed
    // data objects, which hold 
    explicit AttributeStorage(const t_AttrMap &attrMap);

    // Runs destructors of all subtypes
    ~AttributeStorage();

    // Returns a pointer to attribute data object(eg. glm::vec3), 
    // checks for the right type(with typeid) and returns nullptr
    // when something is wrong
    template <class RetType>
    RetType* getData(const t_AttrDesc &loc); 
protected:
    t_StorageType m_storage;
};

template <class RetType>
RetType * AttributeStorage::getData(const t_AttrDesc & loc)
{
    AttributeStorage::t_StorageType resultPtr = m_storage + loc.location;
    if (typeid(resultPtr) != loc.typeInfo)
        return nullptr;

    return static_cast<RetType*> (resultPtr);
}

class CGSEE_API VertexList: public DataBlock
{
    Q_OBJECT
public:

protected:
    QVector<AttributeStorage> m_vertices;
    QList<t_AttrMap> m_attrLayout; 
};

#endif