
#include <core/scenegraph/groundplane.h>

#include <glbinding/gl/enum.h>

#include <core/aabb.h>

#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/polygonaldrawable.h>


Groundplane::Groundplane(std::shared_ptr<DataBlockRegistry> registry)
:   m_polygonalDrawable(new PolygonalDrawable("groundplane"))
,	m_groundplaneGroup(new Group("groundplane"))
,   m_registry(registry)
,	m_drawableInitialized(false)
{
}

Groundplane::~Groundplane()
{
	delete m_polygonalDrawable;
	delete m_groundplaneGroup;
}

void Groundplane::updateGeometry(Group *scene)
{
	const glm::vec3 diff(scene->boundingBox().urb() - scene->boundingBox().llf());
	const glm::vec3 llf(scene->boundingBox().llf());
	const glm::vec3 up(0, 1, 0);
	const glm::mat4 transform;

	const glm::vec3 vertices[4] {
		llf,
			llf + (diff * glm::vec3(1, 0, 0)),
			llf + (diff * glm::vec3(0, 0, 1)),
			llf + (diff * glm::vec3(1, 0, 1))
	};

	const unsigned int indices[6] { 0, 1, 3, 0, 3, 2 };

	m_groundplaneGroup->setTransform(transform);

	for (unsigned int i = 0; i < 4; i++) {
		m_polygonalDrawable->geometry()->setVertex(i, vertices[i]);
		m_polygonalDrawable->geometry()->setNormal(i, up);
	}

	for (unsigned int i = 0; i < 6; i++) {
		m_polygonalDrawable->geometry()->setIndex(i, indices[i]);
	}
}

void Groundplane::initializeDrawable()
{
	auto geometry = std::make_shared<PolygonalGeometry>(m_registry);

	m_polygonalDrawable->setGeometry(geometry);
	m_polygonalDrawable->setMode(gl::GL_TRIANGLES);

	m_groundplaneGroup->append(m_polygonalDrawable);

	m_drawableInitialized = true;
}

Group * Groundplane::getGroundplaneFor(Group *scene)
{
	if (!m_drawableInitialized)
		initializeDrawable();

	updateGeometry(scene);

	return m_groundplaneGroup;
}

Group * Groundplane::getGroundplane()
{
	if (!m_drawableInitialized)
		initializeDrawable();

	return m_groundplaneGroup;
}