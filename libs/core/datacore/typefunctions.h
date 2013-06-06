#pragma once

// The structures and functions found in this file abstract away implementation details of 
// creating, copying and destroying of objects. These details depend on meta object
// system, which is either provided by Qt or designed by the user.
// 

struct AbstractInPlaceTypeFunctions
{
    virtual ~AbstractInPlaceTypeFunctions();
    virtual void construct(void * place) const=0;
    virtual void destruct(void * place) const=0;
    virtual void copyTo(const void * from, void * to) const=0;
    // virtual void moveTo(void * from, void * to) const=0;
};

template <typename T>
struct InPlaceTypeFunctions : public AbstractInPlaceTypeFunctions
{
    void construct(void * place) const
    {
        new(place) T;
    }
    void destruct(void * place) const
    {
        static_cast<T*>(place)->~T();
    }
    void copyTo(const void * from, void * to) const
    {
        new(to) T(static_cast <const T&>(*from));
    }
};

class QtTypeFunctions : public AbstractInPlaceTypeFunctions
{
public:
    QtTypeFunctions(const char * qTypeName);

    void construct(void * place) const;
    void destruct(void * place) const;
    void copyTo(const void * from, void * to) const;

protected:
    int m_typeId;
};