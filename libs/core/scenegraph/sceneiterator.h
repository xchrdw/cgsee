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
    SceneIterator( t_nodeIteratorP it );
    virtual ~SceneIterator();
    
    SceneIterator( const SceneIterator & rhs ); // TODO: evtl = delete;
    // TODO:
//     SceneIterator & operator=( const SceneIterator & rhs );
    
    virtual SceneIterator & operator++() override;
    virtual bool operator ==( t_nodeIteratorP rhs ) const override;
    virtual bool operator !=( t_nodeIteratorP rhs ) const override;
    
    virtual bool operator ==( const INodeIterator & rhs ) const override;
    virtual bool operator !=( const INodeIterator & rhs ) const override;
    
    bool operator ==( const SceneIterator & rhs ) const;
    bool operator !=( const SceneIterator & rhs ) const;
    virtual Node & operator*() const override;
    virtual Node * operator->() const override;
    
    virtual t_nodeIteratorP copy() const override;
    
private:
    typedef struct IteratorPair {
        IteratorPair( t_nodeIteratorP it1, t_nodeIteratorP it2 ) 
        : first(std::move(it1)), second(std::move(it2))
        {}
        t_nodeIteratorP first;
        t_nodeIteratorP second;
    } t_iteratorPair;
    
//     typedef std::pair<t_nodeIteratorP, t_nodeIteratorP > t_iteratorPair;
    typedef std::deque< t_iteratorPair > t_queu;
   
    t_queu iterators_;
};

#endif // SceneIterator_H
