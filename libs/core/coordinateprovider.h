
#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "declspec.h"



class RenderingPass;
class Camera;
class Node;
class Group;


class CGSEE_API CoordinateProvider
{
public:
    CoordinateProvider();
    virtual ~CoordinateProvider();

    unsigned int objID(unsigned int x, unsigned int y);
    glm::dvec3 pointAt(unsigned int x, unsigned int y);

    void assignCamera(Camera * camera);
    void assignPass(RenderingPass * pass);

    void resize(const int width, const int height);

protected:
    glm::dvec3 unproject(float x, float y, float z);

    RenderingPass * m_pass;

};