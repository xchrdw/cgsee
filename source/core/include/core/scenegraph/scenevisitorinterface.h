#pragma once

#include <core/core_api.h>

class Node;

class CORE_API SceneVisitorInterface
{
public:
    virtual ~SceneVisitorInterface() {}
    virtual bool operator() (Node & node) =0;
};
