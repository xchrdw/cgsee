#pragma once

#ifndef DrawVisitor_H
#define DrawVisitor_H

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "node.h"
#include "ISceneVisitor.h"

class CGSEE_API DrawVisitor : public ISceneVisitor
{
public:
    DrawVisitor( Program * p, glm::mat4 t ) : program(p), transform(t)
    {}
    
    virtual void operator() ( Node & node ) override
    {
        node.draw( *program, transform );
        if( Node::RF_Relative == node.referenceFrame() )
            transform *= node.transform();
    }
    
private:
    Program * program;
    glm::mat4 transform;
};


#endif // DrawVisitor_H
