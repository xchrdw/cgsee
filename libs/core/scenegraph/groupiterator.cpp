#include <vector>
#include <map>
#include <iostream>

#include "node.h"
#include "groupiterator.h"

GroupIterator::GroupIterator( QList<Node*>::iterator it ) 
: it_( it ) 
{}

GroupIterator::~GroupIterator(){}

bool GroupIterator::operator==( t_nodeIterator rhs ) const
{ 
    return operator==( *rhs );
}

bool GroupIterator::operator!=( t_nodeIterator rhs ) const
{ 
    return !operator==( *rhs );
};

bool GroupIterator::operator==( const INodeIterator & rhs ) const
{
    const GroupIterator * tmp = dynamic_cast<const GroupIterator*>( &rhs );
    if( tmp ){
        return it_ == tmp->it_;
    }
    return false;
}

bool GroupIterator::operator!=( const INodeIterator & rhs ) const
{
    return !operator==( rhs );
}

GroupIterator & GroupIterator::operator++()
{ 
    ++it_; return *this; 
};

Node & GroupIterator::operator*() const
{ 
    return **it_; 
}

Node * GroupIterator::operator->() const
{ 
    return *it_; 
}

t_nodeIterator GroupIterator::copy() const
{
    return t_nodeIterator( new GroupIterator( *this ) );
}
