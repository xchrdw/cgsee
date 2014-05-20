#pragma once

#include <core/core_api.h>

#include <functional>

class Node;

struct CORE_API SceneTraverser final
{
public:
    void traverse(Node & node, std::function<bool (Node &)> visitor);
};


struct CORE_API ConstSceneTraverser final
{
public:
    void traverse(const Node & node, std::function<bool (const Node &)> visitor);
};
