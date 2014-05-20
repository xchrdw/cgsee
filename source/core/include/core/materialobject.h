#pragma once

#include <core/core_api.h>

#include <reflectionzeug/Color.h>
#include <reflectionzeug/PropertyGroup.h>
#include <glm/vec4.hpp>

class CORE_API MaterialObject : public reflectionzeug::PropertyGroup
{
public:
	MaterialObject();
	virtual ~MaterialObject();

    std::string name() const;
    void setName(const std::string &name);

    glm::vec4 ambient() const;
	void setAmbient(const glm::vec4 &ambient);

	glm::vec4 diffuse() const;
	void setDiffuse(const glm::vec4 &diffuse);
	
	glm::vec4 specular() const;
	void setSpecular(const glm::vec4 &specular);

	glm::vec4 emission() const;
	void setEmission(const glm::vec4 &emission);

    reflectionzeug::Color color() const;
    void setColor(const reflectionzeug::Color &color);

protected:
    std::string           m_name;
	glm::vec4             m_ambient;
	glm::vec4             m_diffuse;
	glm::vec4             m_specular;
	glm::vec4             m_emission;
    reflectionzeug::Color m_color;
};
