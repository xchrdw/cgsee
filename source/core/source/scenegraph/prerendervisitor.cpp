#include <core/scenegraph/prerendervisitor.h>

#include <core/scenegraph/node.h>
#include <core/camera.h>
#include <core/rendering/lightrepresentation/abstractlight.h>
#include <core/rendering/lightingsystem.h>


PreRenderVisitor::PreRenderVisitor(LightingSystem & manager, glm::mat4 t)
: m_manager(manager)
, m_transform(t)
{
}

PreRenderVisitor::~PreRenderVisitor()
{
}

bool PreRenderVisitor::operator() (Node & node)
{
	if (node.hidden())
		return false;
	return true;
}

bool PreRenderVisitor::operator() (Camera & camera)
{
	if (camera.hidden())
		return false;
	camera.setView(m_transform);

	if (Node::RF_Relative == camera.referenceFrame())
		m_transform *= camera.transform();
	return true;
}

bool PreRenderVisitor::operator() (AbstractLight & light)
{
	if (light.hidden())
		return false;

	if (Node::RF_Relative == light.referenceFrame())
		m_transform *= light.transform();

	light.saveLightData(m_manager, m_transform);

	return true;
}