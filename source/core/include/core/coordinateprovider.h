#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

class AbstractCamera;
class Node;
class Group;


class CORE_API CoordinateProvider
{
public:
    CoordinateProvider();
    virtual ~CoordinateProvider();

    virtual unsigned int getObjectID(unsigned int x, unsigned int y) = 0;
    virtual glm::dvec3 get3DPoint(unsigned int x, unsigned int y) = 0;
    virtual glm::ivec2 get2DPoint(unsigned int x, unsigned int y) = 0;
};
