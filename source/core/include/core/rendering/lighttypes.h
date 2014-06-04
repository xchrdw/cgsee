#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

#define MAX_POINT_LIGHTS 12
#define MAX_SPOT_LIGHTS 12

struct DirectionalLight{
	glm::vec3 m_direction;
	float pad0;
	glm::vec3 m_intensity; // Color intensity (HDR)
	float pad1;
};

struct PointLight{
	glm::vec3 m_position;
	float pad;
	glm::vec3 m_intensity; // Color intensity (HDR)
	float pad1;
	glm::vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	float pad2;
};

struct SpotLight{
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_intensity; // Color intensity (HDR)
	float m_angle;
	glm::vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	};

struct GoboLight{
	glm::vec3 m_position;
	glm::vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	glm::vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
};

__declspec(align(16)) struct LightInfo
{
	unsigned int numPointLights;
	unsigned int numSpotLights;
	float pad[2];
	DirectionalLight directionalLight; 
};

__declspec(align(16)) struct PointLightBuffer
{
	PointLight lights[MAX_POINT_LIGHTS];
};