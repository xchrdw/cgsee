#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>

#include <memory>

#include <QString>

#include <glm/glm.hpp>

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>

class DataBlockRegistry;
class PolygonalDrawable;


class CORE_API Groundplane : public Group
{
public:
	Groundplane(Group *scene, std::shared_ptr<DataBlockRegistry> registry = nullptr);
	virtual ~Groundplane();

	Group * getGroundplaneFor(Group *scene);
	Group * getGroundplane();

protected:
	void initializePolygonalDrawable();
	void setGeometryAccoringTo(Group *scene);

protected:
	Group *m_exteriorScene;
	PolygonalDrawable *m_polygonalDrawable;

	std::shared_ptr<DataBlockRegistry> m_registry;
};
