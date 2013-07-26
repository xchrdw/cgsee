
#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "declspec.h"



class AbstractScenePainter;
class Camera;
class Node;
class Group;


class CGSEE_API CoordinateProvider
{
public:
    CoordinateProvider();
    virtual ~CoordinateProvider();

    unsigned int objID(int x, int y);
    glm::vec3 pointAt(int x, int y);

    void assignCamera(Camera * camera);
    void assignScene(Group * rootNode);

protected:
    void initialize();

    AbstractScenePainter * m_painter;
    Group * m_rootNode;

    QVector<Node *> m_nodes;
};