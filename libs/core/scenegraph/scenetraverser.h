#pragma once

#ifndef SceneTraverser_H
#define SceneTraverser_H

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "node.h"

struct CGSEE_API SceneTraverser final
{
public:
    ~SceneTraverser()
    {}
        
    void traverse( Node & node, std::function<void (Node &)> visitor )
    {
        visitor( node );
        
        Node::t_children::const_iterator it = node.children().begin();
        Node::t_children::const_iterator itEnd = node.children().end();
        for( ; it != itEnd; ++it ){
            traverse( **it, visitor );
        }
    }
};


struct CGSEE_API ConstSceneTraverser final
{
public:
    ~ConstSceneTraverser()
    {}
        
    void traverse( const Node & node, std::function<void (const Node &)> visitor )
    {
        visitor( node );
        
        Node::t_children::const_iterator it = node.children().begin();
        Node::t_children::const_iterator itEnd = node.children().end();
        for( ; it != itEnd; ++it ){
            traverse( **it, visitor );
        }
    }
};

#endif // SceneTraverser_H
