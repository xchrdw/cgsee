#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

#define MAX_POINT_LIGHTS 12
#define MAX_SPOT_LIGHTS 12
#define MAX_DIRECTIONAL_LIGHTS 4

/*
	Replicating GLSL padding rules in C++ structs seems to have little use, introduces
	changes in the struct although such changes are just costly in time because changes 
	have to be introduced in seperate places and have no useful return in the end.

	LightingSystem::add* methods do the packing of the data in the exposed interface.
*/

struct SlimDirectionalLight{
	glm::vec4 m_direction;		// xyzw: Direction
	glm::vec4 m_intensity;		// xyz: Color intensity (HDR)
};

struct SlimPointLight{
	glm::vec4 m_position;		// xyzw: Position
	glm::vec4 m_intensity;		// xyz: Color intensity (HDR)
	glm::vec4 m_falloff;		// x: constant falloff, y: linaer falloff, z: quadratic falloff w: radius
};

struct SlimSpotLight{
	glm::vec4 m_position;
	glm::vec4 m_direction;
	glm::vec4 m_intensity;		// Color intensity (HDR)
	glm::vec4 m_falloff;		// x: constant falloff, y: linaer falloff, z: quadratic falloff, w: range
	glm::vec4 m_spotlightData;  	// x: Cone power for pow(dot(l,r)
};

struct LightInfo
{
	uint32_t numPointLights;
	uint32_t numSpotLights;
	uint32_t numDirectionalLights;
	float pad;
};

struct PointLightBuffer
{
	SlimPointLight lights[MAX_POINT_LIGHTS];
};

struct SpotLightBuffer
{
	SlimSpotLight lights[MAX_SPOT_LIGHTS];
};

struct DirectionalLightBuffer
{
	SlimDirectionalLight lights[MAX_DIRECTIONAL_LIGHTS];
};
