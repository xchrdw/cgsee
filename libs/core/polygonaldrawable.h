#pragma once

#include <GL/glew.h>

#include "node.h"

#include <QMap>
#include <QVector>

#include <glm/glm.hpp>


class PolygonalGeometry;
class BufferObject;
class Program;


class PolygonalDrawable : public Node
{
public:
    PolygonalDrawable(const QString & name = "unnamed");
    virtual ~PolygonalDrawable();

    void setGeometry(PolygonalGeometry * geometry);
    PolygonalGeometry * geometry();

    virtual void draw(
        Program * program
    ,   const glm::mat4 & transform);

    virtual const AxisAlignedBoundingBox boundingBox() const;

protected:

    const glm::mat4 & transform() const;

	void setupBuffers();
    void deleteBuffers();

    virtual void invalidateBoundingBox();

protected:
    GLuint m_vao;

    PolygonalGeometry * m_geometry;

	typedef QVector<BufferObject *> t_bufferObjects;
	t_bufferObjects m_elementArrayBOs;

	typedef QMap<QString, BufferObject *> t_bufferObjectsByAttribute;
	t_bufferObjectsByAttribute m_arrayBOsByAttribute;	
};