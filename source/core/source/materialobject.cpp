#include <core/materialobject.h>
//ambient
//diffuse
//specular
//emission

MaterialObject::MaterialObject()
: reflectionzeug::PropertyGroup("MaterialObject")
, m_name("default")
, m_ambient(glm::vec4(0.1, 0.1, 0.1, 1.0))
, m_diffuse(glm::vec4(1.0, 1.0, 1.0, 1.0))
, m_specular(glm::vec4(1.0, 1.0, 1.0, 1.0))
, m_emission(glm::vec4(0.0, 0.0, 0.0, 0.0))
, m_color(0, 255, 0, 255)
{
	addProperty<std::string>("Name", this, &MaterialObject::name, &MaterialObject::setName);

	//addProperty<glm::vec4>("Ambient", this, &MaterialObject::ambient, &MaterialObject::setAmbient);

	//addProperty<glm::vec4>("Diffuse", this, &MaterialObject::diffuse, &MaterialObject::setDiffuse);

	//addProperty<glm::vec4>("Specular", this, &MaterialObject::specular, &MaterialObject::setSpecular);

	//addProperty<glm::vec4>("Emission", this, &MaterialObject::emission, &MaterialObject::setEmission);

	addProperty<reflectionzeug::Color>("Color", this, &MaterialObject::color, &MaterialObject::setColor);
}

MaterialObject::~MaterialObject()
{
}

std::string MaterialObject::name() const
{
    return m_name;
}

void MaterialObject::setName(const std::string &name)
{
    m_name = name;
}

glm::vec4 MaterialObject::ambient() const
{
	return m_ambient;
}

void MaterialObject::setAmbient(const glm::vec4 &ambient)
{
	m_ambient = ambient;
}

glm::vec4 MaterialObject::diffuse() const
{
	return m_diffuse;
}

void MaterialObject::setDiffuse(const glm::vec4 &diffuse)
{
	m_diffuse = diffuse;
}

glm::vec4 MaterialObject::specular() const
{
	return m_specular;
}

void MaterialObject::setSpecular(const glm::vec4 &specular)
{
	m_specular = specular;
}

glm::vec4 MaterialObject::emission() const
{
	return m_emission;
}

void MaterialObject::setEmission(const glm::vec4 &emission)
{
	m_emission = emission;
}

reflectionzeug::Color MaterialObject::color() const
{
    return m_color;
}

void MaterialObject::setColor(const reflectionzeug::Color &color)
{
    m_color = color;
}
