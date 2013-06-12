#pragma once

#ifndef NullIterator_H
#define NullIterator_H

#include <cassert>

#include <core/declspec.h>

#include "INodeIterator.h"


class Node;

class CGSEE_API NullIterator : public INodeIterator
{
public:
    
    NullIterator() 
    {}
    
    virtual ~NullIterator() {}
    
    virtual NullIterator & operator++() override 
    { 
        return *this; 
    }

    virtual bool operator==( t_nodeIterator rhs ) const override
    { 
        return operator==( *rhs ); 
    }
    virtual bool operator!=( t_nodeIterator rhs ) const override
    { 
        return !operator==( *rhs ); 
    }
    
    virtual bool operator==( const INodeIterator & rhs ) const
    {
        const NullIterator * tmp = dynamic_cast<const NullIterator*>( &rhs );
        if( tmp )
            return true;
        return false;
    }
    
    virtual bool operator!=( const INodeIterator & rhs ) const
    {
        return !operator==( rhs );
    }
    
    virtual Node * operator->() const override
    { assert(false); }
    
    virtual Node & operator*() const override
    { assert(false); }

    virtual t_nodeIterator copy() const override
    { 
        return t_nodeIterator( new NullIterator() ); 
    }
};

#endif // NullIterator_H
