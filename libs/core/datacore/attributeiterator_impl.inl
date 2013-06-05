#include <cassert>

template <typename T>
AttributeIterator<T>::AttributeIterator()
{

}

template <typename T>
AttributeIterator<T>::AttributeIterator(const AttributeIterator<T>& rhs):
    m_owner(rhs.m_owner)
    ,   m_currentIndex(rhs.m_currentIndex)
    ,   m_attrDesc(rhs.m_attrDesc)
    ,   m_typeChecked(rhs.m_typeChecked)
{

}

template <typename T>
AttributeIterator<T>::~AttributeIterator()
{

}

template <typename T>
bool AttributeIterator<T>::operator==(AttributeIterator<T> const & rhs) const
{
    return (m_owner == rhs.m_owner && m_currentIndex == rhs.m_currentIndex)
        || (isInvalid() && rhs.isInvalid());
}

template <typename T>
bool AttributeIterator<T>::operator !=(AttributeIterator<T> const & rhs) const
{
    return !operator ==(rhs);
}

template<typename T>
AttributeIterator<T>& AttributeIterator<T>::operator++()
{
    ++m_currentIndex;
    return *this;
}

template <typename T>
AttributeIterator<T> AttributeIterator<T>::operator++(int)
{
    AttributeIterator<T> temp(*this);
    ++m_currentIndex;
    return temp;
}

template <typename T>
typename AttributeIterator<T>::reference
    AttributeIterator<T>::operator*()
{
    AttributeStorage & storage = m_owner->m_vertices.at(index);
    if (!m_typeChecked)
    {
        m_typeChecked = storage.checkDataType<T>(m_attrDesc);
        assert(m_typeChecked);
    }
    return *storage.getDataUnchecked<T>(m_attrDesc);
}

template <typename T>
typename AttributeIterator<T>::pointer
    AttributeIterator<T>::operator->()
{
    AttributeStorage & storage = m_owner->m_vertices.at(index);
    if (!m_typeChecked)
    {
        m_typeChecked = storage.checkDataType<T>(m_attrDesc);
        assert(m_typeChecked);
    }
    return storage.getDataUnchecked<T>(m_attrDesc);
}

template <typename T>
bool AttributeIterator<T>::isInvalid() const
{
    return !m_owner || m_currentIndex >= m_owner->size();
}

template <typename T>
void AttributeIterator<T>::_initialize(t_VertexListP owner, unsigned int index , t_AttrDesc const & attrDesc)
{
    m_owner = owner;
    m_currentIndex = index;
    m_attrDesc = attrDesc;
    m_typeChecked = false;
}
