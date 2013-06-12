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
    SceneIterator( t_nodeIterator it );
    virtual ~SceneIterator();
    
    SceneIterator( const SceneIterator & rhs ); // TODO: evtl = delete;
    // TODO:
//     SceneIterator & operator=( const SceneIterator & rhs );
    
    virtual SceneIterator & operator++() override;
    virtual bool operator ==( t_nodeIterator rhs ) const override;
    virtual bool operator !=( t_nodeIterator rhs ) const override;
    
    virtual bool operator ==( const INodeIterator & rhs ) const override;
    virtual bool operator !=( const INodeIterator & rhs ) const override;
    
    bool operator ==( const SceneIterator & rhs ) const;
    bool operator !=( const SceneIterator & rhs ) const;
    virtual Node & operator*() const override;
    virtual Node * operator->() const override;
    
    virtual t_nodeIterator copy() const override;
    
private:
    typedef std::deque< std::pair<t_nodeIterator, t_nodeIterator > > t_queu;
   
    t_queu iterators_;
};

#endif // SceneIterator_H
