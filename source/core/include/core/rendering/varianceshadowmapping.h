#pragma once

#include <core/core_api.h>

#include <glm/mat4x4.hpp>

#include <core/rendering/defaultpass.h>

class CORE_API  VarianceShadowMappingPass : public DefaultPass
{
public:
	VarianceShadowMappingPass(Camera * camera);
	virtual ~VarianceShadowMappingPass(void);

	virtual void render() override;
	virtual void setUniforms() override;
	virtual void resize(const int width, const int height) override;
    void sceneChanged(Group * scene) override;

private:
	static const glm::mat4 biasMatrix;
	//std::vector<Camera *> lightCam;
	Camera *m_lightCamera;
	Camera *m_light2Camera;

	Program *m_lightProgram;

	FrameBufferObject* m_shadowmapFBO3D;
	FrameBufferObject* m_shadowmapFBO;
	FrameBufferObject* m_shadowmap2FBO;

    Group *m_scene;
};