#pragma once

#ifndef NodeIterator_H
#define NodeIterator_H

#include <QList>
#include <memory>

#include <core/declspec.h>

#include "INodeIterator.h"


class Node;

class CGSEE_API NodeIterator : public INodeIterator
{
public:
    NodeIterator( QList<Node*>::iterator it );
    virtual ~NodeIterator();
    
    virtual NodeIterator & operator++() override;
    virtual bool operator==( IteratorType rhs ) const override;
    virtual bool operator!=( IteratorType rhs ) const override;
    virtual bool operator==( const INodeIterator & rhs ) const override;
    virtual bool operator!=( const INodeIterator & rhs ) const override;
    virtual Node * operator->() const override;
    virtual Node & operator*() const override;

    virtual IteratorType copy() const override;
    
private:
    QList<Node*>::iterator it_;
};

#endif // NodeIterator_H
