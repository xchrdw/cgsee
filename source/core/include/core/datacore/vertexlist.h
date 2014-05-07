#pragma once

#include <typeinfo>
#include <memory>
#include <functional>

#include "typefunctions.h"
#include "datablock.h"

#include "registertypeshere.h"

// A VertexList object is a storage of vertices, each defined by a heterogeneous 
// collection of its attributes. Attributes of a single vertex are represented 
// by an AttributeStorage object. VertexList stores internally an array of 
// AttributeStorage objects and provides an interface to access their content.
// 
// A freshly created VertexList object has to be initialized with a list 
// of AttributeSpec objects, that determine attribute types that are to be stored
// as well as names of each attribute, that are necessary to access data later.
// 

/// Determines how much memory (in bytes) will be used for attributes of a 
/// vertex. 
/// == sizeof(AttributeStorage)   -- not yet
const int StaticAttributeStorageSize = 64;

// describes layout of vertex attributes in a storage.
typedef struct AttributeDescriptor
{
    unsigned int location;
    unsigned int size;
    QString typeName;
    std::shared_ptr<AbstractInPlaceTypeFunctions> factory;
    std::type_info const* typeInfo;
    unsigned int typeId;
    bool used;
} t_AttrDesc;

struct CORE_API AttributeSpec
{
    QString attrName;
    QString attrType;

    AttributeSpec(const QString &name, const QString &type);
};

typedef QHash<QString, t_AttrDesc> t_AttrMap;
template <typename T> class AttributeIterator;

class CORE_API VertexList: public DataBlock
{
    Q_OBJECT
public:
    typedef VertexList * t_StandardPointer;
    typedef VertexList const * t_StandardPointerToConst;

    // A vertex (and everything really) can be described by its attributes.
    // This class provides a storage for them, but it doesn't memorize the right layout.
    // Which is not needed anyway, as most of the times many vertices have same attribute types.
    class CORE_API AttributeStorage final
    {
    public:
        typedef unsigned char t_StorageType[StaticAttributeStorageSize];

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
        template <class RetType>
        RetType const* getData(const t_AttrDesc &loc) const; 

        template <class RetType>
        RetType* getDataUnchecked(const t_AttrDesc &loc);
        template <class RetType>
        RetType const* getDataUnchecked(const t_AttrDesc &loc) const;

        template <class RetType>
        bool checkDataType(const t_AttrDesc &loc) const;
    protected:
        t_StorageType m_storage;
        //unsigned int m_storageSize;
        //bool m_initialized;

        //mutable unsigned int * m_useCount;
        QPointer<VertexList> m_owner; // todo: move it somewhere else

        void copyStorage(t_StorageType const& otherStorage);
    };

    template <typename T>
    struct const_iterator
    {
        typedef AttributeIterator<const T> type;
    };

    template <typename T>
    struct iterator
    {
        typedef AttributeIterator<T> type;
    };

    ~VertexList();

    void initialize(const QList<AttributeSpec> &attrTypes);

    // access functions
    // template parameters == C++ attribute types
    // 1. set/get a single attribute value of a single vertex
    template <class RetType>
    RetType * getVertexAttribute(int index, const QString &attrName);

    // get a specified attribute of many vertices
    template <class T>
    void foreachVertexAttribute(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<bool(int)> select // can be null
        ,   std::function<void(int, const T&)> inject);

    // set a specified attribute of many vertices
    template <class T>
    void setVertexAttributes(int startIndex
        ,   int endIndex
        ,   const QString &attrName
        ,   std::function<void (int, T&)> setter);

    // Iterator-related functions
    template <class T>
    typename iterator<T>::type begin(const QString &attrName);

    template <class T>
    typename const_iterator<T>::type begin(const QString &attrName) const;

    template <class T>
    typename const_iterator<T>::type cbegin(const QString &attrName) const;

    template <class T>
    typename iterator<T>::type end(const QString &attrName);

    template <class T>
    typename const_iterator<T>::type end(const QString &attrName) const;

    template <class T>
    typename const_iterator<T>::type cend(const QString &attrName) const;

    void createNewVertices(unsigned int amount);

    // some utility functions
    unsigned int size() const;
    bool isEmpty() const;
    bool isAttributeUsed(QString attrName) const;
    void resize(unsigned int);

    friend class AttributeStorage;
    friend class DataBlock;
    template <typename T> friend class AttributeIterator; 
protected:
    explicit VertexList(QObject* parent = nullptr);

    QVector<AttributeStorage> m_vertices;
    t_AttrMap m_attrLayout; 
    bool m_initialized;

    const t_AttrMap& getAttrMap();
    virtual t_StandardPointer createClone() override;
};

typedef VertexList::t_StandardPointer t_VertexListP;
typedef VertexList::t_StandardPointerToConst t_ConstVertexListP;

#include "vertexlist_impl.tli"
#include "attributeiterator.h"
