#pragma once

#include <core/core_api.h>

#include <functional>

class Node;

template < class NodeType >
struct SceneTraverserTemplate
{
public:
	void traverse(NodeType & node, std::function<bool(NodeType &)> visitor);
};

using SceneTraverser = SceneTraverserTemplate<Node>;
using ConstSceneTraverser = SceneTraverserTemplate<const Node>;

template < class NodeType >
void SceneTraverserTemplate<NodeType>::traverse(NodeType & node, std::function<bool(NodeType &)> visitor)
{
	if (!visitor(node))
		return;

	auto it = node.children().begin();
	auto itEnd = node.children().end();
	for (; it != itEnd; ++it){
		traverse(**it, visitor);
	}
}

