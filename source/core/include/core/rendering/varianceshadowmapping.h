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
	virtual FrameBufferObject* output() override;

private:
	static const glm::mat4 biasMatrix;
	Camera *m_lightCamera;

	Program *m_lightProgram;
	Program *m_shadowProgram;

	FrameBufferObject* m_shadowmapFBO;
	FrameBufferObject* m_shadowFBO;
};