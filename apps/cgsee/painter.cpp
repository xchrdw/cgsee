


#include "painter.h"

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/group.h>
#include <core/objio.h>
#include <core/program.h>
#include <core/screenquad.h>

//for phong, flat and gouraud
static const QString CAMERAPOSITION_UNIFORM   ("cameraposition");
static const QString LIGHTDIR_UNIFORM   ("lightdir");
static const QString LIGHTDIR_UNIFORM2   ("lightdir2");
static const QString ATTENUATION_UNIFORM   ("attCoeff");
static const QString ATTENUATION_UNIFORM2   ("attCoeff2");
static const QString LIGHTAMBIENTGLOBAL_UNIFORM   ("lightambientglobal");
static const QString LIGHT_UNIFORM   ("light");
static const QString LIGHT_UNIFORM2   ("light2");
static const QString LIGHTSHININESS_UNIFORM   ("lightshininess");
static const QString MATERIAL_UNIFORM   ("material");
//for gooch
static const QString LIGHTPOSITION_UNIFORM   ("lightposition");

Painter::Painter()
:   AbstractPainter()
,   m_group(nullptr)
,   m_quad(nullptr)
,   m_normalz(nullptr)
,   m_flat(nullptr)
,   m_gouraud(nullptr)
,   m_phong(nullptr)
,   m_gooch(nullptr)
,   m_useProgram(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_camera(nullptr)
{
}

Painter::~Painter()
{
    delete m_group;
    delete m_quad;

    delete m_normalz;
	delete m_flat;
	delete m_gouraud;
	delete m_phong;
	delete m_gooch;
    delete m_fboNormalz;
    delete m_flush;    
}

Camera * Painter::camera()
{
    return m_camera;
}

const bool Painter::initialize()
{
    AutoTimer t("Initialization of Painter");

    m_group = ObjIO::groupFromObjFile("data/suzanneVN.obj");

    if(!m_group)
    {
        qWarning("Have you set the Working Directory?");
        return false;
    }

    glm::mat4 transform(1.f);

    transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));

    m_group->setTransform(transform);

    // Camera Setup

    m_camera = new Camera();

    m_camera->setFovy (45.0f);
    m_camera->setZNear( 1.0f);
    m_camera->setZFar (10.0f);

    m_camera->append(m_group);

	camPos=glm::vec3( 1.8f, 0.5f,-2.f);
    m_camera->setView(glm::lookAt(
        camPos , glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));

    m_quad = new ScreenQuad();

    // G-Buffer Shader

	//FLAT
	m_normalz = new Program();
	m_normalz->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
	m_normalz->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));

	m_flat = new Program();
	m_flat->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flat.frag"));
	m_flat->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/flat.vert"));
	//m_flat->attach(
	//	new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/flat.geo"));

	//GOURAUD
	
	m_gouraud = new Program();
	m_gouraud->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/gouraud.frag"));
	m_gouraud->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/gouraud.vert"));

	//PHONG

	m_phong = new Program();
	m_phong->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/phong.frag"));
	m_phong->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/phong.vert"));

	//GOOCH
	m_gooch = new Program();
	m_gooch->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/gooch.frag"));
	m_gooch->attach(
	 	new FileAssociatedShader(GL_VERTEX_SHADER, "data/gooch.vert"));

	//set UNIFORMS for seleced shader
	m_useProgram = m_gouraud;

	if(m_useProgram != m_gooch)
	{
		m_useProgram->setUniform(CAMERAPOSITION_UNIFORM, camPos);
		m_useProgram->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,4.5,7.5));
		m_useProgram->setUniform(ATTENUATION_UNIFORM, glm::vec3(0.002,0.002,0.0004));
	
		m_useProgram->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,0.0,7.5));
		m_useProgram->setUniform(ATTENUATION_UNIFORM2, glm::vec3(0.002,0.002,0.0004));

		glm::mat4 lightMat;
		lightMat[0] = glm::vec4(0.1,0.1,0.1,1.0);	//ambient
		lightMat[1] = glm::vec4(0.7,0.7,0.0,0.0);	//diffuse
		lightMat[2] = glm::vec4(0.3,0.5,0.7,1.0);	//specular
		lightMat[3] = glm::vec4(0,0,0,0);			//nichts?
		m_useProgram->setUniform(LIGHT_UNIFORM, lightMat, true); 

		glm::mat4 lightMat2;
		lightMat[0] = glm::vec4(0.2,0.2,0.2,1.0);	//ambient
		lightMat[1] = glm::vec4(0.7,0.0,0.0,1.0);	//diffuse
		lightMat[2] = glm::vec4(0.2,0.1,0.5,1.0);	//specular
		lightMat[3] = glm::vec4(0,0,0,0);			//nichts?
	
		m_useProgram->setUniform(LIGHT_UNIFORM2, lightMat2, true); 

		float shin=1.4f;
		m_useProgram->setUniform(LIGHTSHININESS_UNIFORM, shin);
	
		m_useProgram->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.4));

		glm::mat4 materialCoeff;
		materialCoeff[0] = glm::vec4(0.1,0.14,0.1,1.0);	//ambient
		materialCoeff[1] = glm::vec4(1.0,0.9,1.0,1.0);	//diffuse
		materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);	//specular
		materialCoeff[3] = glm::vec4(0,0,0,0);			//emission

		m_useProgram->setUniform(MATERIAL_UNIFORM, materialCoeff);
	}

	else
	{
		m_gooch->setUniform(LIGHTPOSITION_UNIFORM, glm::vec3(0.0,4.5,7.5));
	}

    // Post Processing Shader

    m_flush = new Program();
	m_flush->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
	m_flush->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));

    m_fboNormalz = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    return true;
}

void Painter::paint()
{
    AbstractPainter::paint();

	t_samplerByName sampler;
    m_camera->setView(glm::lookAt(
        camPos, glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));


	// Normals and Depth to RGBA

    //m_camera->draw(m_normalz, m_fboNormalz);
	//m_camera->draw(m_flat, m_fboNormalz);
	//m_camera->draw(m_gouraud, m_fboNormalz);
	m_camera->draw(*m_useProgram, m_fboNormalz);

    sampler.clear();
	sampler["source"] = m_fboNormalz;

	bindSampler(sampler, *m_flush);
    m_quad->draw(*m_flush, nullptr);
	releaseSampler(sampler);

}

void Painter::resize(
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);
    
    m_camera->setViewport(width, height);

    m_fboNormalz->resize(width, height);

	postShaderRelinked();

}

void Painter::postShaderRelinked()
{
}

void Painter::bindSampler(

    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void Painter::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();

}
