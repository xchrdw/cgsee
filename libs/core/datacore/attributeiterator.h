#pragma once
#include <iterator>

// AttributeIterator iterates over vertices in a VertexList object, providing
// an attribute specified by its creation. Use it like this:
// 
// auto myIterator = vList.begin<glm::vec3> ("position");
// auto endIterator = vList.end<glm::vec3> ("position");
// 
// for (; myIterator != endIterator; ++myIterator)
// {
//     (*myIterator) = glm::vec3();
// }
// 
// ...

template <typename T>
class AttributeIterator final
    : public std::iterator<std::input_iterator_tag, T>
{
public:
    AttributeIterator();
    AttributeIterator(AttributeIterator<T> const &);
    ~AttributeIterator();
    typedef typename std::iterator<std::input_iterator_tag, T>::pointer pointer;
    typedef typename std::iterator<std::input_iterator_tag, T>::reference reference;

    bool operator ==(AttributeIterator<T> const & rhs) const;
    bool operator !=(AttributeIterator<T> const & rhs) const;

    // prefix increment
    AttributeIterator<T>& operator++();
    AttributeIterator<T> operator++(int);

    reference operator*();
    pointer operator->();

    bool isInvalid() const;

    friend class VertexList;
private:
    // internal func; only to be called from VertexList class
    void _initialize(t_VertexListP owner, unsigned int index
        ,   t_AttrDesc const & attrDesc);
protected:
    t_VertexListP m_owner;
    unsigned int m_currentIndex;
    t_AttrDesc const & m_attrDesc;
    bool m_typeChecked;
};

#include "attributeiterator_impl.inl"