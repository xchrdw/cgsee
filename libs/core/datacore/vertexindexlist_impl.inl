#pragma once
#include "vertexlist.h"

template <class T>
void VertexIndexList::foreachVertexAttribute(int startIdx, int endIdx, const QString &attrName, std::function<bool(int)> select, std::function<void (int, const T&)> inject)
{
    assert(m_associatedList);

    for(int i = startIdx; i < endIdx; ++i)
    {
        if ((!select) || select(i))
        {
            T* temp = m_associatedList->getVertexAttribute<T>(m_indices[i], attrName);
            if (temp)
                inject(i, *temp);
        }
    }
}

template <class T>
void VertexIndexList::foreachTriangle(int startIndex, int endIndex, const QString &attrName, std::function<void(int, const T&, const T&, const T&)> func) const
{
    assert(m_associatedList);
    assert((endIndex - startIndex) % 3 == 0);
    assert(endIndex <= m_indices.size());
    for (int idx = startIndex; idx < endIndex; idx += 3)
    {
        const T * par1 = m_associatedList->getVertexAttribute<T>(m_indices[idx], attrName);
        const T * par2 = m_associatedList->getVertexAttribute<T>(m_indices[idx+1], attrName);
        const T * par3 = m_associatedList->getVertexAttribute<T>(m_indices[idx+2], attrName);
        if (par1 && par2 && par3)
            func(idx, *par1, *par2, *par3);
    }
}