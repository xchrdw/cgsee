#pragma once

#ifndef SceneTraverser_H
#define SceneTraverser_H

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "node.h"

class CGSEE_API SceneTraverser
{
public:
    typedef std::vector<std::shared_ptr<Node> >::iterator t_nodeIter;

    virtual ~SceneTraverser()
    {}
        
    virtual void traverse( Node & node, std::function<void (Node &)> visitor )
    {
        Node::t_children::const_iterator it = node.children().begin();
        Node::t_children::const_iterator itEnd = node.children().end();
        for( ; it != itEnd; ++it ){
            traverse( **it, visitor );
        }
        visitor( node );
    }
};


#endif // SceneTraverser_H
