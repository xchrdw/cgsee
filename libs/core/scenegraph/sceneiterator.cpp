
#include <cassert>

#include "node.h"
#include "nulliterator.h"
#include "sceneiterator.h"


SceneIterator::SceneIterator( IteratorType it )
: iterators_()
{
    assert( it );
    
    iterators_.push_back( {std::move(it), IteratorType(new NullIterator())} );
}

SceneIterator::~SceneIterator()
{
    
}

SceneIterator::SceneIterator( const SceneIterator & rhs )
: iterators_()
{
    t_queu::const_iterator it = rhs.iterators_.begin();
    t_queu::const_iterator itEnd = rhs.iterators_.end();
    for(; it != itEnd; ++it )
        iterators_.push_back({ it->first->copy(), it->second->copy() });
}

bool SceneIterator::operator ==( IteratorType rhs ) const 
{ 
    return *(iterators_.back().first) == *rhs;
}

bool SceneIterator::operator !=( IteratorType rhs ) const
{
    return !operator==( std::move(*rhs) );
}

bool SceneIterator::operator==( const INodeIterator & rhs ) const
{
    return *(iterators_.back().first) == rhs;
}

bool SceneIterator::operator!=( const INodeIterator & rhs ) const
{
    return !operator==( rhs );
}

bool SceneIterator::operator ==( const SceneIterator & rhs ) const { 
    return *(iterators_.back().first) == *(rhs.iterators_.back().first);
}

bool SceneIterator::operator !=( const SceneIterator & rhs ) const { 
    return !operator==( rhs );
}

SceneIterator & SceneIterator::operator++()
{
    iterators_.push_back( {(*iterators_.back().first)->begin(), (*iterators_.back().first)->end()} );
    while( iterators_.size() > 1 && *(iterators_.back().first) == *(iterators_.back().second) )
    {
        iterators_.pop_back();
        ++(*iterators_.back().first);
    }
    return *this;
}

Node & SceneIterator::operator*() const 
{
    return **(iterators_.back().first);
}

Node * SceneIterator::operator->() const
{
    return &(**(iterators_.back().first));
}

IteratorType SceneIterator::copy() const
{
    return IteratorType( new SceneIterator( *this ) );
}
