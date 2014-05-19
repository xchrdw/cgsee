#pragma once

#include <glm/glm.hpp>

using namespace glm;

typedef struct DirectionalLight{
	vec3 m_direction;
}directionalLight;

typedef struct PointLight{
	vec3 m_position;
	vec3 m_intensity;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
}pointLight;

typedef struct SpotLight{
	vec3 m_position;
	vec3 m_direction;
	vec3 m_intensity;
	float m_angle;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
}spotLight;

typedef struct GoboLight{
	vec3 m_position;
	vec3 m_direction;
	//	texture* m_gobo_texture;
	float m_angle;
	float m_kquad;
	float m_klin;
	float m_kcons;
	bool enabled;
}goboLight;

typedef struct AreaLight{

}areaLight