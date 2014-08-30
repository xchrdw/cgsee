#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>

#include <memory>

#include <QString>

#include <glm/glm.hpp>

#include <core/scenegraph/node.h>

class DataBlockRegistry;
class Group;
class PolygonalDrawable;


class CORE_API Groundplane
{
public:
	Groundplane(std::shared_ptr<DataBlockRegistry> registry = nullptr);
	virtual ~Groundplane();

	Group * getGroundplaneFor(Group *scene);
	Group * getGroundplane();

protected:
	void initializeDrawable();
	void updateGeometry(Group *scene);

protected:
	PolygonalDrawable * m_polygonalDrawable;
	Group * m_groundplaneGroup;
	
	bool m_drawableInitialized;

	std::shared_ptr<DataBlockRegistry> m_registry;
};
