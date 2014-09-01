#include <core/material/material.h>

#include <core/material/image.h>
#include <core/program.h>

#include <reflectionzeug/extensions/GlmProperties.hpp>
//ambient
//diffuse
//specular
//emission

Material::Material()
: reflectionzeug::PropertyGroup("Material")
/*
, m_name("default")
, m_ambient(glm::vec4(0.1, 0.1, 0.1, 1.0))
, m_diffuse(glm::vec4(1.0, 1.0, 1.0, 1.0))
, m_specular(glm::vec4(1.0, 1.0, 1.0, 1.0))
, m_emission(glm::vec4(0.0, 0.0, 0.0, 0.0))
, m_color(0, 255, 0, 255)
*/
{
	/*
	addProperty<std::string>("Name", this, &Material::name, &Material::setName);

	addProperty<glm::vec4>("Ambient", this, &Material::ambient, &Material::setAmbient);

	addProperty<glm::vec4>("Diffuse", this, &Material::diffuse, &Material::setDiffuse);

	addProperty<glm::vec4>("Specular", this, &Material::specular, &Material::setSpecular);

	addProperty<glm::vec4>("Emission", this, &Material::emission, &Material::setEmission);

	addProperty<reflectionzeug::Color>("Color", this, &Material::color, &Material::setColor);
	*/
}

Material::~Material()
{
    m_textures.clear();
}

void Material::addTexture(enum aiTextureType textureType, Image* texture) {
    if (texture != nullptr && texture->isValid())
        m_textures.insert(textureType, std::shared_ptr<Image>(texture));
};

void Material::bind(const Program & program) {
    bindTextures(program);
}

void Material::bindTextures(const Program & program) {
    bindTexture(program, aiTextureType::aiTextureType_DIFFUSE, "Diffuse", 5);
    bindTexture(program, aiTextureType::aiTextureType_SPECULAR, "Specular", 6);
}

void Material::bindTexture(const Program & program, aiTextureType type, QString name, GLubyte textureUnit) {
    if (m_textures.count(type) != 0) {
        std::shared_ptr<Image> texture = m_textures.find(type).value();
        texture->bind(program, QString("material.").append(name.toLower()), textureUnit);
        program.setUniform(QString("material.use").append(name), true);
    }
    else {
        program.setUniform(QString("material.use").append(name), false);
    }
}

/*
std::string Material::name() const
{
    return m_name;
}

void Material::setName(const std::string &name)
{
    m_name = name;
}

glm::vec4 Material::ambient() const
{
	return m_ambient;
}

void Material::setAmbient(const glm::vec4 &ambient)
{
	m_ambient = ambient;
}

glm::vec4 Material::diffuse() const
{
	return m_diffuse;
}

void Material::setDiffuse(const glm::vec4 &diffuse)
{
	m_diffuse = diffuse;
}

glm::vec4 Material::specular() const
{
	return m_specular;
}

void Material::setSpecular(const glm::vec4 &specular)
{
	m_specular = specular;
}

glm::vec4 Material::emission() const
{
	return m_emission;
}

void Material::setEmission(const glm::vec4 &emission)
{
	m_emission = emission;
}

reflectionzeug::Color Material::color() const
{
    return m_color;
}

void Material::setColor(const reflectionzeug::Color &color)
{
    m_color = color;
}
*/
