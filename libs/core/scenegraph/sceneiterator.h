#pragma once

#ifndef SceneIterator_H
#define SceneIterator_H

#include <deque>
#include <utility>

#include <core/declspec.h>

#include "INodeIterator.h"

class CGSEE_API SceneIterator : public INodeIterator
{
public:
    SceneIterator( IteratorType it );
    virtual ~SceneIterator();
    
    SceneIterator( const SceneIterator & rhs ); // TODO: evtl = delete;
    // TODO:
//     SceneIterator & operator=( const SceneIterator & rhs );
    
    virtual SceneIterator & operator++() override;
    virtual bool operator ==( IteratorType rhs ) const override;
    virtual bool operator !=( IteratorType rhs ) const override;
    
    virtual bool operator ==( const INodeIterator & rhs ) const override;
    virtual bool operator !=( const INodeIterator & rhs ) const override;
    
    bool operator ==( const SceneIterator & rhs ) const;
    bool operator !=( const SceneIterator & rhs ) const;
    virtual Node & operator*() const override;
    virtual Node * operator->() const override;
    
    virtual IteratorType copy() const override;
    
private:
//     typedef std::deque< IteratorType > t_queu;
    typedef std::deque< std::pair<IteratorType, IteratorType > > t_queu;
    
    t_queu iterators_;
//     std::vector< std::unique_ptr<IteratorType> > iterators_;
};

#endif // SceneIterator_H
