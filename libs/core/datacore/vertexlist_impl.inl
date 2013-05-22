#pragma once

template <class RetType>
RetType * AttributeStorage::getData(const t_AttrDesc & loc)
{
    if (loc.size + loc.location > StaticAttributeStorageSize)
        return nullptr;
    unsigned char* resultPtr = m_storage + loc.location;
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
    m_attrLayout[attrName].used = true;
    return m_vertices[index].getData<RetType>(m_attrLayout[attrName]);
}

template <class T>
void VertexList::foreachVertexAttribute(int startIdx, int endIdx, const QString &attrName, std::function<bool(int)> select, std::function<void (int, const T&)> inject)
{
    if (!m_initialized)
        return;

    assert(m_vertices.size() >= endIdx);
    assert(m_attrLayout.contains(attrName));

    for (int i = startIdx; i < endIdx; ++i)
    {
        if ((!select) || select(i))
        {
            T const* temp = m_vertices[i].getData<T>(m_attrLayout[attrName]);
            assert(temp != nullptr);
            inject(i, *temp);
        }
    }
}

template <class T>
void VertexList::setVertexAttributes(int startIndex, int endIndex, const QString &attrName, std::function<void(int, T&)> setter)
{
    if (!m_initialized)
        return;

    assert(m_attrLayout.contains(attrName));
    m_attrLayout[attrName].used = true;
    if (m_vertices.size() < endIndex)
    {
        createNewVertices(endIndex - m_vertices.size());
    }

    for (int i = startIndex; i < endIndex; ++i)
    {
        T * temp = m_vertices[i].getData<T>(m_attrLayout[attrName]);
        assert (temp != nullptr);
        setter(i, *temp);
    }
}

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