
#include <core/rendering/varianceshadowmapping.h>

#include <glm/gtc/matrix_transform.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>
#include <core/aabb.h>

#include <cmath>
#include <vector>


namespace
{
    const int ShadowMapSize = 256;
    const int LAYER_COUNT = 4;
    const float PI = 3.14159265358979323846f;

	const glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	std::array<glm::vec3, 8> calculatePointsOfViewFrustum(Camera *camera)
	{
		std::array<glm::vec3, 8> result;
		glm::vec3 up = glm::normalize(camera->getUp());
		glm::vec3 eye = camera->getEye();
		glm::vec3 direction = glm::normalize(camera->getCenter() - eye);
		float zFar = camera->zFar();
		float zNear = camera->zNear();

		glm::vec3 right = glm::normalize(glm::cross(direction, up));

		glm::vec3 farPlaneCenter = eye + direction * zFar;
		glm::vec3 nearPlaneCenter = eye + direction * zNear;

		float nearHeight = std::tan(camera->fovy() * PI / 360.f) * camera->zNear();
		float nearWidth = nearHeight * camera->aspect();
		float farHeight = std::tan(camera->fovy() * PI / 360.f) * camera->zFar();
		float farWidth = farHeight * camera->aspect();

		result[0]=(nearPlaneCenter - up * nearHeight - right * nearWidth);
		result[1]=(nearPlaneCenter + up * nearHeight - right * nearWidth);
		result[2]=(nearPlaneCenter + up * nearHeight + right * nearWidth);
		result[3]=(nearPlaneCenter - up * nearHeight + right * nearWidth);
		result[4]=(farPlaneCenter - up * farHeight - right * farWidth);
		result[5]=(farPlaneCenter + up * farHeight - right * farWidth);
		result[6]=(farPlaneCenter + up * farHeight + right * farWidth);
		result[7]=(farPlaneCenter - up * farHeight + right * farWidth);

		return result;
	}
}

VarianceShadowMappingPass::VarianceShadowMappingPass(Camera * camera)
: DefaultPass(camera)
, m_lightProgram(new Program())
 {
	m_lightProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_light.frag"));
    m_lightProgram->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shadows/vsm_light.geom"));
	m_lightProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_light.vert"));

	m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_shadow.frag"));
	m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_shadow.vert"));

	m_shadowmapFBO3D = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, 4, true);
	m_shadowmapFBO3D->resize(ShadowMapSize, ShadowMapSize);
}

VarianceShadowMappingPass::~VarianceShadowMappingPass()
{
	delete(m_lightProgram);
	delete(m_shadowmapFBO3D);
}


std::list<glm::vec2> VarianceShadowMappingPass::calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda)
{
    std::list<glm::vec2> result;
    for (float i = 0; i < planesCount; ++i)
    {
        glm::vec2 borders;
        borders.x = lambda * (znear * pow(zfar / znear, i / planesCount)) + (1.f - lambda) * (znear + (zfar - znear) * (i / planesCount));
        borders.y = lambda * (znear * pow(zfar / znear, (i + 1.f) / planesCount)) + (1.f - lambda) * (znear + (zfar - znear) * ((i + 1.f) / planesCount));
        result.push_back(borders);
    }
    return result;
}

std::list<glm::mat4x4> VarianceShadowMappingPass::calculateCropMatrices(std::list<glm::vec2> splitPlanes, Camera *lightCamera)
{
	std::list<glm::mat4x4> result;

	float zNear = m_camera->zNear();
	float zFar = m_camera->zFar();

	std::list<glm::vec2>::const_iterator iterator;
    for (iterator = splitPlanes.begin(); iterator != splitPlanes.end(); ++iterator)
	{
		m_camera->setZNear(iterator->x);
		m_camera->setZFar(iterator->y);

		std::array<glm::vec3, 8> points = calculatePointsOfViewFrustum(m_camera);

        std::array<glm::vec3, 8>::const_iterator pointsBegin = points.begin();
        std::array<glm::vec3, 8>::const_iterator pointsEnd = points.end();

		glm::vec4 firstVertex = lightCamera->transform() * glm::vec4(*pointsBegin, 1.0);
        glm::vec3 minValues ( (firstVertex / firstVertex.w).xyz );
        glm::vec3 maxValues = minValues;

		for (++pointsBegin; pointsBegin != pointsEnd; ++pointsBegin)
        {
			glm::vec4 vertex = lightCamera->transform() * glm::vec4(*pointsBegin, 1.0);
            vertex /= vertex.w;
            minValues.x = glm::min(minValues.x, vertex.x);
            minValues.y = glm::min(minValues.y, vertex.y);
            minValues.z = glm::min(minValues.z, vertex.z);

            maxValues.x = glm::max(maxValues.x, vertex.x);
            maxValues.y = glm::max(maxValues.y, vertex.y);
            maxValues.z = glm::max(maxValues.z, vertex.z);
        }
		minValues = glm::clamp(minValues, glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.f, 1.f, 1.f));
		maxValues = glm::clamp(maxValues, glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.f, 1.f, 1.f));
		glm::mat4x4 cropMatrix = glm::ortho(minValues.x, maxValues.x, minValues.y, maxValues.y, 1.f, -1.f);
		result.push_back(cropMatrix);
	}
	m_camera->setZFar(zFar);
	m_camera->setZNear(zNear);
	return result;
}

void VarianceShadowMappingPass::render()
{
    AxisAlignedBoundingBox bb = m_camera->asGroup()->boundingBox();

    Camera lightCamera = Camera();
    lightCamera.setZNear(m_camera->zNear());
    lightCamera.setZFar(20.f);
    lightCamera.setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
    lightCamera.setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f) + bb.center(),
        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
	lightCamera.setFovy(90.f);
	lightCamera.update();

	std::list<glm::vec2> split = calculateSplitPlanes(m_camera->zNear(), m_camera->zFar(), 4, 0.5);
	glm::vec4 farDistances;
	int i;
	std::list<glm::vec2>::iterator it;
	for (it = split.begin(), i = 0; it != split.end(); ++it, ++i)
	{
		switch (i)
		{
		case 0: farDistances.x = it->y; break;
		case 1: farDistances.y = it->y; break;
		case 2: farDistances.z = it->y; break;
		case 3: farDistances.w = it->y; break;
		default:
			break;
		}
	}
	std::list<glm::mat4x4> cropMatrices = calculateCropMatrices(split, &lightCamera);
	
	glm::mat4x4 *lightViewProjection = new glm::mat4x4[4];
	glm::mat4x4 *biasLightViewProjection = new glm::mat4x4[4];

	std::list<glm::mat4x4>::const_iterator iterator;
	for ( i = 0, iterator = cropMatrices.begin() ; iterator != cropMatrices.end(); ++iterator, ++i)
	{
		glm::mat4x4 cropMatrix = *iterator;
		lightViewProjection[i] = cropMatrix * lightCamera.transform();
		biasLightViewProjection[i] = biasMatrix * lightViewProjection[i];
	}

    m_lightProgram->setUniform("lightTransforms", 4, lightViewProjection[0]);
    m_lightProgram->setUniform("inverseViewProjection", m_camera->transformInverse());
    drawScene(m_camera, m_lightProgram, m_shadowmapFBO3D);

	m_program->setUniform("inverseViewProjection", m_camera->transformInverse());
	m_program->setUniform("biasLightViewProjection", 4, biasLightViewProjection[0]);
	m_program->setUniform("farDistances", farDistances);

	m_shadowmapFBO3D->bindTexture3D(*m_program, "shadowmap3D", 0);
	m_program->setUniform("inverseProjection", glm::inverse(m_camera->projection()));
	drawScene(m_camera, m_program, m_fbo);

    delete[] biasLightViewProjection;
    delete[] lightViewProjection;
}

void VarianceShadowMappingPass::resize(const int width, const int height)
{
	DefaultPass::resize(width, height);
    m_camera->update();
}