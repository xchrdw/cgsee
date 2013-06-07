#pragma once
#include <iterator>
#include <type_traits>

// Some small helpers

// if_then_else chooses one or the other type depending on trait's value
template <typename trait, typename ifTrue, typename ifFalse>
struct if_then_else
{
    typedef typename if_then_else<typename trait::type, ifTrue, ifFalse>::type
        type;
};

template <typename ifTrue, typename ifFalse>
struct if_then_else<std::true_type, ifTrue, ifFalse>
{
    typedef ifTrue type;
};

template <typename ifTrue, typename ifFalse>
struct if_then_else<std::false_type, ifTrue, ifFalse>
{
    typedef ifFalse type;
};


// clear_const removes outer const qualifiers
template <typename T>
struct clear_const
{
    typedef T type;
};

template <typename T>
struct clear_const <T const>
{
    typedef T type;
};


// And is only true if both arguments are true
template <typename T1, typename T2>
struct logic_and : public std::false_type
{
    typedef std::false_type::type type;
    typedef std::false_type::value_type value_type;
};

template <>
struct logic_and <std::true_type, std::true_type>: public std::true_type
{
    typedef std::true_type::type type;
    typedef std::true_type::value_type value_type;
};

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

    template <typename Trhs
        ,   typename = typename std::enable_if<! std::is_const<Trhs>::value>::type
        ,   typename = typename std::enable_if<std::is_same<T, const Trhs>::value>::type>
    struct _enable_conversion
    {
        typedef void* type;
    };

    template <typename Trhs>
    AttributeIterator(AttributeIterator<Trhs> const& rhs
                    , typename _enable_conversion<Trhs>::type j = nullptr);

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
    template <typename Trhs> friend class AttributeIterator;
private:
    typedef typename if_then_else<std::is_const<T>
        ,   t_ConstVertexListP
        ,   t_VertexListP>::type    t_OwnerType;

    // If this is const iterator, we should copy attribute descriptor somewhere else 
    // and dispose of it at the end.
    // Otherwise we use the object given to us.
    typedef typename if_then_else<std::is_const<T>
        ,   std::shared_ptr<t_AttrDesc>
        ,   t_AttrDesc *>::type   t_AttrDescType;

    // internal func; only to be called from VertexList class
    void _initialize(t_ConstVertexListP owner, unsigned int index
        ,   t_AttrDesc const & attrDesc);
    void _initialize(t_VertexListP owner, unsigned int index
        ,   t_AttrDesc & attrDesc);
protected:
    t_OwnerType m_owner;
    unsigned int m_currentIndex;
    t_AttrDescType m_attrDesc;
    bool m_typeChecked;
};

#include "attributeiterator_impl.inl"