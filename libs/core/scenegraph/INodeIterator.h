#pragma once

#ifndef INodeIterator_H
#define INodeIterator_H

#include <memory>

#include <core/declspec.h>

class Node;
class INodeIterator;


typedef std::unique_ptr<INodeIterator> t_nodeIterator;

class CGSEE_API INodeIterator
{
public:
    virtual INodeIterator & operator++() =0;
    
    virtual bool operator==( t_nodeIterator rhs ) const =0;
    virtual bool operator!=( t_nodeIterator rhs ) const =0;
    
    virtual bool operator==( const INodeIterator & rhs ) const =0;
    virtual bool operator!=( const INodeIterator & rhs ) const =0;
    
    virtual Node * operator->() const =0;
    virtual Node & operator*() const =0;
    
    virtual t_nodeIterator copy() const =0;
};



#endif // INodeIterator_H
