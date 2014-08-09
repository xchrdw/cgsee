#pragma once
#include "core/rendering/lighttypes.h"

#include <vector>

#include <glbinding/gl/types.h>

#include <core/core_api.h>

struct LightUniformBuffers
{
	enum
	{
		Point,
		Spot,
		Directional,
		Info,
		// Shadowed point/spec lights might require new buffer
		Count
	};
	gl::GLuint ubos[LightUniformBuffers::Count];
    gl::GLuint bindingLocations[LightUniformBuffers::Count];
    uint32_t sizes[LightUniformBuffers::Count];
};

// Rename to LightingSystem in a seperate commit
class CORE_API LightingSystem
{
public:
	LightingSystem();
	virtual ~LightingSystem();

	/*
		The whole system is designed that these vectors hold structs which are 16-byte aligned.
		This reflects the way GPU uniform buffers work as well, in 16-byte(that is, one vec4) granularity.
		So, pack only glm::vec4's inside your buffers.
	*/
	std::vector<PointLight> m_pointLights;
	std::vector<SpotLight> m_spotLights;
	std::vector<DirectionalLight> m_directionalLights;

	void addDirectionalLight(const glm::vec3& direction, const glm::vec3& intensity);
	void addPointLight(const glm::vec4& pos, const glm::vec3& intensity, float radius);
	void addSpotLight(const glm::vec4& pos, const glm::vec3& direction, const glm::vec3& intensity, float range, float conePower);

	void initBuffers();
	// Use the enum in LightUBO to update specific buffers
    void updateBuffer(uint8_t buffer_enum, gl::GLuint activeProgram, void* data);
    void updateAllBuffers(gl::GLuint activeProgram);
	void bindBuffer(uint8_t buffer_enum);
	void bindAllBuffers();

    void onShaderRelink(gl::GLuint relinkedProgram);

protected:
	LightInfo m_lightInfo;
	LightUniformBuffers m_uniformBuffers;
};
