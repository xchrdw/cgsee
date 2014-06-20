#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

#define MAX_POINT_LIGHTS 12
#define MAX_SPOT_LIGHTS 12

struct DirectionalLight{
	glm::vec3 m_direction;
	float pad0;
	glm::vec3 m_intensity;		// Color intensity (HDR)
	float pad1;
};

struct PointLight{
	glm::vec4 m_position;
	glm::vec4 m_intensity;		// Color intensity (HDR)
	glm::vec4 m_falloff;		// x = constant falloff, y = linaer falloff, z = quadratic falloff
};

struct SpotLight{
	glm::vec3 m_position;
	float pad0;
	glm::vec3 m_direction;
	float pad1;
	glm::vec3 m_intensity;		// Color intensity (HDR)
	float pad2;
	glm::vec3 m_falloff;		// x = constant falloff, y = linaer falloff, z = quadratic falloff
	float m_spotFalloffPower;	// Cone power for pow(dot(l,r)
	float m_dist;
	float pad3[3];
};

struct GoboLight{
	glm::vec3 m_position;
	glm::vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	glm::vec3 m_falloff;		// x = constant falloff, y = linaer falloff, z = quadratic falloff
};

__declspec(align(16)) struct LightInfo
{
	uint32_t numPointLights;
	uint32_t numSpotLights;
	float pad[2];
	DirectionalLight directionalLight; 
};

__declspec(align(16)) struct PointLightBuffer
{
	PointLight lights[MAX_POINT_LIGHTS];
};

__declspec(align(16)) struct SpotLightBuffer
{
	SpotLight lights[MAX_SPOT_LIGHTS];
};