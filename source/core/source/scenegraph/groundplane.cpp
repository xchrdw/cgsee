
#include <core/scenegraph/groundplane.h>

#include <glbinding/gl/enum.h>

#include <core/aabb.h>

#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/polygonaldrawable.h>


Groundplane::Groundplane(Group *scene, std::shared_ptr<DataBlockRegistry> registry)
:   Group("Groundplane")
,   m_polygonalDrawable(new PolygonalDrawable("Groundplane"))
,   m_exteriorScene(scene)
,   m_registry(registry)
{
	initializePolygonalDrawable();
	setGeometryAccoringTo(scene);
}

Groundplane::~Groundplane()
{
}

void Groundplane::initializePolygonalDrawable()
{
	auto geometry = std::make_shared<PolygonalGeometry>(m_registry);

	m_polygonalDrawable->setGeometry(geometry);
	m_polygonalDrawable->setMode(gl::GL_TRIANGLES);

	Group::append(m_polygonalDrawable);
}

void Groundplane::setGeometryAccoringTo(Group *scene)
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

	Group::setTransform(transform);

	for (unsigned int i = 0; i < 4; i++) {
		m_polygonalDrawable->geometry()->setVertex(i, vertices[i]);
		m_polygonalDrawable->geometry()->setNormal(i, up);
	}

	for (unsigned int i = 0; i < 6; i++) {
		m_polygonalDrawable->geometry()->setIndex(i, indices[i]);
	}
}