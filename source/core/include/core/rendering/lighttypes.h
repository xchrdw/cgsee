#pragma once

#include <glm/glm.hpp>
#include <stdint.h>

using namespace glm;

struct LightInfo
{
	uint32_t numPointLights;
	uint32_t numSpotLights;
	DirectionalLight directionalLight;
};

struct DirectionalLight{
	vec3 m_direction;
	vec3 m_intensity; // Color intensity (HDR)
};

struct PointLight{
	vec3 m_position;
	vec3 m_intensity; // Color intensity (HDR)
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	bool enabled;
};

struct SpotLight{
	vec3 m_position;
	vec3 m_direction;
	vec3 m_intensity; // Color intensity (HDR)
	float m_angle;
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	bool enabled;
};

struct GoboLight{
	vec3 m_position;
	vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
//	bool enabled;
};

struct AreaLight{

};