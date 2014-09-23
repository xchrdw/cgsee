#pragma once

#include <core/core_api.h>
#include <core/gpuquery.h>

#include <reflectionzeug/Color.h>
#include <reflectionzeug/PropertyGroup.h>
#include <glm/vec4.hpp>

#include <QMultiMap>

#include <assimp/scene.h>

class Image;
class Program;

class CORE_API Material : public reflectionzeug::PropertyGroup
{
public:
	Material();
	virtual ~Material();

    void addTexture(enum aiTextureType textureType, Image* texture);
    void bind(const Program & program);
    void bindTextures(const Program & program);
    void bindTexture(const Program & program, aiTextureType type, QString name, GLubyte textureUnit);

protected:
    QMultiMap<enum aiTextureType, std::shared_ptr<Image>> m_textures;
	/*
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
	*/
};
