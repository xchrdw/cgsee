
#include <core/scenegraph/groundplane.h>

#include <glbinding/gl/enum.h>

#include <core/aabb.h>

#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/polygonaldrawable.h>


Groundplane::Groundplane(std::shared_ptr<DataBlockRegistry> registry)
: m_registry(registry)
{
}

Groundplane::~Groundplane()
{
	delete m_groundplaneGroup;
}

void Groundplane::updateGeometry(Group *scene)
{
	PolygonalDrawable *polygonalDrawable = new PolygonalDrawable("groundplane");

	auto geometry = std::make_shared<PolygonalGeometry>(m_registry);

	polygonalDrawable->setGeometry(geometry);
	polygonalDrawable->setMode(gl::GL_TRIANGLES);

	m_groundplaneGroup = new Group("groundplane");
	m_groundplaneGroup->append(polygonalDrawable);

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
		polygonalDrawable->geometry()->setVertex(i, vertices[i]);
		polygonalDrawable->geometry()->setNormal(i, up);
	}

	for (unsigned int i = 0; i < 6; i++) {
		polygonalDrawable->geometry()->setIndex(i, indices[i]);
	}
}

Group * Groundplane::getGroundplaneFor(Group *scene)
{
	updateGeometry(scene);

	return m_groundplaneGroup;
}

Group * Groundplane::getGroundplane()
{
	return m_groundplaneGroup;
}