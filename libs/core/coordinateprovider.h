
#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "declspec.h"



class AbstractPainter;
class Node;


class CGSEE_API CoordinateProvider
{
public:
    CoordinateProvider();
    virtual ~CoordinateProvider();

    int objID(int x, int y);
    glm::vec3 pointAt(int x, int y);

    void assignScene(Node * rootNode);

protected:
    void initialize();

    AbstractPainter * m_painter;
    Node * m_rootNode;

    QVector<Node *> m_nodes;
};