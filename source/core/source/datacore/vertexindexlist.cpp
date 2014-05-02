
#include <core/datacore/vertexindexlist.h>

VertexIndexList::VertexIndexList(QObject* parent)
{
    if (parent)
    {
        VertexList* myList = qobject_cast<VertexList*>(parent);
        if (myList)
            setVertexList(myList);
    }
}

void VertexIndexList::setVertexList(t_VertexListP associatedList)
{
    m_associatedList = associatedList;
    if (associatedList)
    {
        connect(associatedList, SIGNAL(updated(QObject*)), SLOT(onVertexListUpdated()));
        connect(associatedList, SIGNAL(destroyed(QObject*)), SLOT(onVertexListDestroyed()));
    }
}

QVector<VertexIndexList::t_indexType> const& VertexIndexList::getIndices() const
{
    return m_indices;
}

void VertexIndexList::setSingleIndex(unsigned int pos, VertexIndexList::t_indexType const vindex)
{
    if (static_cast<unsigned int>(m_indices.size()) <= pos)
        m_indices.resize(pos + 1);
    m_indices[pos] = vindex;
}

void VertexIndexList::setMultipleIndices(unsigned int start, unsigned int end, std::function<t_indexType(unsigned int)> initFunc)
{
    for (unsigned int i = start; i < end; ++i)
    {
        m_indices[i] = initFunc(i);
    }
}

void VertexIndexList::onVertexListUpdated()
{

}

void VertexIndexList::onVertexListDestroyed()
{
    m_associatedList = nullptr;
}

unsigned int VertexIndexList::size() const
{
    return m_indices.size();
}

bool VertexIndexList::isEmpty() const
{
    return m_indices.isEmpty();
}

void VertexIndexList::resize(unsigned int s)
{
    m_indices.resize(s);
}

t_VertexIndexListP VertexIndexList::createClone()
{
    return new VertexIndexList(*this);
}