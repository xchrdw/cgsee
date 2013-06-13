#pragma once

#ifndef NodeIterator_H
#define NodeIterator_H

#include <QList>
#include <memory>

#include <core/declspec.h>

#include "INodeIterator.h"


class Node;

class CGSEE_API GroupIterator : public INodeIterator
{
public:
    GroupIterator( QList<Node*>::iterator it );
    virtual ~GroupIterator();
    
    virtual GroupIterator & operator++() override;
    virtual bool operator==( t_nodeIteratorP rhs ) const override;
    virtual bool operator!=( t_nodeIteratorP rhs ) const override;
    virtual bool operator==( const INodeIterator & rhs ) const override;
    virtual bool operator!=( const INodeIterator & rhs ) const override;
    virtual Node * operator->() const override;
    virtual Node & operator*() const override;

    virtual t_nodeIteratorP copy() const override;
    
private:
    QList<Node*>::iterator it_;
};

#endif // NodeIterator_H
