#pragma once
#include <functional>

#include "datablock.h"

class VertexList;
typedef VertexList* t_VertexListP;

class CGSEE_API VertexIndexList : public DataBlock
{
    Q_OBJECT
public:
    typedef unsigned int t_indexType;
    typedef VertexIndexList * t_StandardPointer;

    void setVertexList(t_VertexListP associatedList);
    QVector<t_indexType> const& getIndices() const;
    void setSingleIndex(unsigned int pos, t_indexType const vindex);
    void setMultipleIndices(unsigned int start, unsigned int end
        ,   std::function<t_indexType(unsigned int)> initFunc);

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
    void resize(unsigned int);

    friend class AttributeStorage;
    friend class DataBlock;

    protected slots:
        void onVertexListUpdated();
        void onVertexListDestroyed();
protected:
    VertexIndexList(QObject* parent = nullptr);
    virtual t_StandardPointer createClone() override;

    t_VertexListP m_associatedList;
    QVector<unsigned int> m_indices;
};

typedef VertexIndexList::t_StandardPointer t_VertexIndexListP;

#include "vertexindexlist_impl.inl"