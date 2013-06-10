#include <vector>
#include <map>
#include <iostream>

#include "node.h"
#include "nodeiterator.h"

NodeIterator::NodeIterator( QList<Node*>::iterator it ) 
: it_( it ) 
{}

NodeIterator::~NodeIterator(){}

bool NodeIterator::operator==( IteratorType rhs ) const
{ 
    return operator==( *rhs );
}

bool NodeIterator::operator!=( IteratorType rhs ) const
{ 
    return !operator==( *rhs );
};

bool NodeIterator::operator==( const INodeIterator & rhs ) const
{
    const NodeIterator * tmp = dynamic_cast<const NodeIterator*>( &rhs );
    if( tmp ){
        return it_ == tmp->it_;
    }
    return false;
}

bool NodeIterator::operator!=( const INodeIterator & rhs ) const
{
    return !operator==( rhs );
}

NodeIterator & NodeIterator::operator++()
{ 
    ++it_; return *this; 
};

Node & NodeIterator::operator*() const
{ 
    return **it_; 
}

Node * NodeIterator::operator->() const
{ 
    return *it_; 
}

IteratorType NodeIterator::copy() const
{
    return IteratorType( new NodeIterator( *this ) );
}
