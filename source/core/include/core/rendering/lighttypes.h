#pragma once

#include <glm/glm.hpp>

using namespace glm;

struct directionalLight{
	vec3 m_direction;
};

struct pointLight{
	vec3 m_position;
	vec3 m_intensity;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
};

struct spotLight{
	vec3 m_position;
	vec3 m_direction;
	vec3 m_intensity;
	float m_angle;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
};

struct goboLight{
	vec3 m_position;
	vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
};

struct areaLight{

};