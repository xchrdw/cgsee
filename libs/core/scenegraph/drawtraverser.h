#pragma once

#ifndef DrawTraverser_H
#define DrawTraverser_H

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "node.h"

class CGSEE_API DrawTraverser
{
public:
    typedef std::vector<std::shared_ptr<Node> >::iterator t_nodeIter;
    
    ~DrawTraverser()
    {}
    
    void traverse( Camera & camera, const Program & program, FrameBufferObject * target )
    {
        camera.draw( program, target );
        
        Node::t_children::const_iterator it = camera.children().begin();
        Node::t_children::const_iterator itEnd = camera.children().end();
        for( ; it != itEnd; ++it ){
            traverse__( **it, program, camera.transform() );
        }
        
        if(target)
            target->release();
    }
    
protected:
    void traverse__( Node & node, const Program & program, const glm::mat4 & transform )
    {
        node.draw( program, transform );
        
        Node::t_children::const_iterator it = node.children().begin();
        Node::t_children::const_iterator itEnd = node.children().end();
        for( ; it != itEnd; ++it ){
            if( Node::RF_Absolute == node.referenceFrame() )
                traverse__( **it, program, transform );
            else
                traverse__( **it, program, transform * node.transform() );
        }
    }
};


#endif // DrawTraverser_H
