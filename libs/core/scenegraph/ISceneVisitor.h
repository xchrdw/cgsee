#pragma once

#include <core/declspec.h>

class Node;

class CGSEE_API ISceneVisitor
{
public:
    virtual ~ISceneVisitor() {}
    virtual bool operator() ( Node & node ) =0;
};
