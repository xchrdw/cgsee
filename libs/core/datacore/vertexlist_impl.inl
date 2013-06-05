#pragma once

template <class RetType>
RetType * VertexList::AttributeStorage::getData(const t_AttrDesc & loc)
{
    unsigned char* resultPtr = m_storage + loc.location;
    if (checkDataType<RetType>(loc))
        return reinterpret_cast<RetType*> (resultPtr);

    return nullptr;
}

template <class RetType>
RetType * VertexList::AttributeStorage::getDataUnchecked(const t_AttrDesc & loc)
{
    unsigned char* resultPtr = m_storage + loc.location;

    return reinterpret_cast<RetType*> (resultPtr);
}

template <class RetType>
bool VertexList::AttributeStorage::checkDataType(const t_AttrDesc & loc)
{
    if (loc.size + loc.location > StaticAttributeStorageSize)
        return false;
    if (nullptr == loc.typeInfo)
    {
        if (loc.typeId == qMetaTypeId<RetType>())
            return true;
        else
            return false;
    }

    unsigned char* resultPtr = m_storage + loc.location;
    if (typeid(resultPtr) != *loc.typeInfo)
        return false;

    return true;
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
typename VertexList::const_iterator<T>::type 
    VertexList::end( const QString &attrName ) const
{
    typedef typename VertexList::const_iterator<T>::type return_type;

    return return_type();
}

template <class T>
typename VertexList::iterator<T>::type 
    VertexList::end( const QString &attrName )
{
    typedef typename VertexList::iterator<T>::type return_type;

    return return_type();
}

template <class T>
typename VertexList::const_iterator<T>::type 
    VertexList::begin( const QString &attrName ) const
{
    typedef typename VertexList::const_iterator<T>::type return_type;

    assert(m_attrLayout.contains(attrName));
    return_type iter;
    iter._initialize(this, 0, m_attrLayout[attrName]);

    return iter;
}

template <class T>
typename VertexList::iterator<T>::type 
    VertexList::begin( const QString &attrName )
{
    typedef typename VertexList::iterator<T>::type return_type;

    assert(m_attrLayout.contains(attrName));
    return_type iter;
    iter._initialize(this, 0, m_attrLayout[attrName]);

    return iter;
}