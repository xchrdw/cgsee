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
    bool used;
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
    void foreachVertexAttribute(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<bool(int)> select // can be null
        ,   std::function<void(int, const T&)> inject);

    template <class T>
    void setVertexAttributes(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<void (int, T&)> setter);

    void createNewVertices(unsigned int amount);

    unsigned int size() const;
    bool isEmpty() const;
    bool isAttributeUsed(QString attrName) const;

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

class CGSEE_API VertexIndexList : public DataBlock
{
    Q_OBJECT
public:
    typedef unsigned int t_indexType;
    typedef VertexIndexList * t_StandardPointer;

    void setVertexList(t_VertexListP associatedList);
    QVector<t_indexType> const& getIndices() const;
    void setSingleIndex(unsigned int pos, t_indexType const vindex);
    void setMultipleIndices(unsigned int start, unsigned int end, std::function<t_indexType(unsigned int)> initFunc);
    
    // Calls the same function from VertexList
    template <class T>
    void foreachVertexAttribute(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<bool(int)> select // can be null
        ,   std::function<void(int, const T&)> inject);

    template <class T>
    void foreachTriangle(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<void(int, const T&, const T&, const T&)> func) const;

    unsigned int size() const;
    bool isEmpty() const;
    
    friend class AttributeStorage;
    friend class DataBlock;

protected slots:
    void onVertexListUpdated();
    void onVertexListDestroyed();
protected:
    VertexIndexList(QObject* parent = nullptr);

    t_VertexListP m_associatedList;
    QVector<unsigned int> m_indices;
};

typedef VertexIndexList::t_StandardPointer t_VertexIndexListP;

#include "vertexlist_impl.inl"

#endif