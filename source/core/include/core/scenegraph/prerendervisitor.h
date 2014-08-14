#pragma once

#include <core/core_api.h>
#include <core/scenegraph/scenevisitorinterface.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;
class LightProxy;
class LightingSystem;

class CORE_API PreRenderVisitor : public SceneVisitorInterface
{
public:
	PreRenderVisitor(LightingSystem & manager, glm::mat4 transform);
	~PreRenderVisitor();

	bool operator() (Node & node) override;
	bool operator() (Camera & camera);
	bool operator() (LightProxy & light);
	
private:
	LightingSystem & m_manager;
	glm::mat4 m_transform;
};