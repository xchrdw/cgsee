#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

#define MAX_POINT_LIGHTS 12
#define MAX_SPOT_LIGHTS 12

using namespace glm;

struct DirectionalLight{
	vec3 m_direction;
	vec3 m_intensity; // Color intensity (HDR)
};

struct PointLight{
	vec3 m_position;
	float pad;
	vec3 m_intensity; // Color intensity (HDR)
	float pad1;
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	float pad2;
};

struct SpotLight{
	vec3 m_position;
	vec3 m_direction;
	vec3 m_intensity; // Color intensity (HDR)
	float m_angle;
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
	};

struct GoboLight{
	vec3 m_position;
	vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	vec3 m_falloff;	// x = constant falloff, y = linaer falloff, z = quadratic falloff
};

struct LightInfo
{
	unsigned int numPointLights;
	unsigned int numSpotLights;
	DirectionalLight directionalLight;
};

__declspec(align(16)) struct PointLightBuffer
{
	PointLight lights[MAX_POINT_LIGHTS];
};