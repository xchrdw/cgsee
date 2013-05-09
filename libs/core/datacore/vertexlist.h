#ifndef VERTEXLIST_H
#define VERTEXLIST_H
#pragma once

#include <typeinfo>
#include <memory>
#include <functional>

#include "typefunctions.h"
#include "abstractdata.h"

#include "registertypeshere.h"

// describes layout of vertex attributes in a storage.
typedef struct AttributeDescriptor
{
    unsigned int location;
    unsigned int size;
    QString typeName;
    std::shared_ptr<AbstractInPlaceTypeFunctions> factory;
    type_info const* typeInfo;
    unsigned int typeId;
} t_AttrDesc;

struct CGSEE_API AttributeSpec
{
    QString attrName;
    QString attrType;

    AttributeSpec(const QString &name, const QString &type);
};

typedef QHash<QString, t_AttrDesc> t_AttrMap;
class VertexList;

// A vertex (and everything really) can be described by its attributes.
// This class provides a storage for them, but it doesn't memorize the right layout.
// Which is not needed anyway, as most of the times many vertecies have same attribute types.
class CGSEE_API AttributeStorage final
{
public:
    typedef unsigned char * t_StorageType;

    AttributeStorage();
    AttributeStorage(AttributeStorage &&rhs);
    // Initializes attributeStorage and creates in-place default constructed
    // data objects, which hold vertex attributes
    explicit AttributeStorage(VertexList &owner);

    // copy constructor and assignment.
    AttributeStorage(const AttributeStorage&);
    AttributeStorage const& operator =(const AttributeStorage&);

    ~AttributeStorage();

    void initialize(VertexList &owner);

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
    QPointer<VertexList> m_owner;

    void copyStorage();
};

template <class RetType>
RetType * AttributeStorage::getData(const t_AttrDesc & loc)
{
    if (m_destroyed)
        return nullptr;

    copyStorage();
    AttributeStorage::t_StorageType resultPtr = m_storage + loc.location;
    if (nullptr == loc.typeInfo)
    {
        if (loc.typeId == qMetaTypeId<RetType>())
            return reinterpret_cast<RetType*> (resultPtr);
        else
            return nullptr;
    }

    if (typeid(resultPtr) != *loc.typeInfo)
        return nullptr;

    return reinterpret_cast<RetType*> (resultPtr);
}

class CGSEE_API VertexList: public DataBlock
{
    Q_OBJECT
public:
    typedef VertexList * t_StandardPointer;

    ~VertexList();

    void initialize(const QList<AttributeSpec> &attrTypes);

    template <class RetType>
    RetType * getVertexAttribute(int index, const QString &attrName);

    template <class T>
    void injectVertexAttributes(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<bool(int)> select
        ,   std::function<void(int, const T&)> inject);

    template <class T>
    void setVertexAttributes(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<void (int, T&)> setter);

    void createNewVertices(unsigned int amount);

    friend class AttributeStorage;
    friend class DataBlock;
protected:
    explicit VertexList(QObject* parent = nullptr);

    QVector<AttributeStorage> m_vertices;
    t_AttrMap m_attrLayout; 
    bool m_initialized;

    const t_AttrMap& getAttrMap();
    virtual t_StandardPointer createClone() override;
};

typedef VertexList::t_StandardPointer t_VertexListP;

#include "vertexlist_impl.inl"

#endif