#pragma once
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

    return m_vertices[index].getData<RetType>(m_attrLayout[attrName]);
}

template <class T>
void VertexList::injectVertexAttributes(int startIdx, int endIdx, const QString &attrName, std::function<bool(int)> select, std::function<void (int, const T&)> inject)
{
    if (!m_initialized)
        return;

    assert(m_vertices.size() >= endIdx);
    assert(m_attrLayout.contains(attrName));

    for (int i = startIdx; i < endIdx; ++i)
    {
        if ((!select) || select(i))
        {
            T const* temp = m_vertices[index].getData<T>(m_attrLayout[attrName]);
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
    if (m_vertices.size() < endIndex)
    {
        createNewVertices(endIndex - m_vertices.size());
    }

    for (int i = startIndex; i < endIndex; ++i)
    {
        T * temp = m_vertices[index].getData<T>(m_attrLayout[attrName]);
        assert (temp != nullptr);
        setter(i, *temp);
    }
}