#include <core/rendering/shadowingstage.h>

#include <cmath>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glow/glow.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include <core/camera/abstractcamera.h>
#include <core/camera/monocamera.h>
#include <core/camera/projection.h>
#include <core/fileassociatedshader.h>
#include <core/painter/pipelinepainter.h>
#include <core/program.h>
#include <core/screenquad.h>


namespace
{
    const int SHADOWMAP_SIZE = 512;
    const int LAYER_COUNT = 4;
    const int SHADOWMAP_COUNT = 2;
    const float PI = 3.14159265358979323846f;

    const glm::mat4 biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );
}

ShadowingStage::ShadowingStage(PipelinePainter &painter)
:   AbstractSceneRenderStage(painter)
,   m_shadowmaps(glow::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_shadowmapsDepth(glow::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurTexture(glow::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurHProgram(new Program())
,   m_blurVProgram(new Program())
,   m_blurHFBO(new glow::FrameBufferObject())
,   m_blurVFBO(new glow::FrameBufferObject())
{
    m_program->attach(new FileAssociatedShader(gl::GL_FRAGMENT_SHADER, "data/shadows/vsm_light.frag"));
    m_program->attach(new FileAssociatedShader(gl::GL_GEOMETRY_SHADER, "data/shadows/vsm_light.geom"));
    m_program->attach(new FileAssociatedShader(gl::GL_VERTEX_SHADER, "data/shadows/vsm_light.vert"));

    m_blurHProgram->attach(new FileAssociatedShader(gl::GL_VERTEX_SHADER, "data/screenquad.vert"));
    m_blurHProgram->attach(new FileAssociatedShader(gl::GL_GEOMETRY_SHADER, "data/shadows/screenquad_layered.geom"));
    m_blurHProgram->attach(new FileAssociatedShader(gl::GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_h_layered.frag"));
    
    m_blurVProgram->attach(new FileAssociatedShader(gl::GL_VERTEX_SHADER, "data/screenquad.vert"));
    m_blurVProgram->attach(new FileAssociatedShader(gl::GL_GEOMETRY_SHADER, "data/shadows/screenquad_layered.geom"));
    m_blurVProgram->attach(new FileAssociatedShader(gl::GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_v_layered.frag"));

    m_shadowmaps->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_shadowmapsDepth->image3D(0, gl::GL_DEPTH_COMPONENT32, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_INT, nullptr);
    m_blurTexture->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);    

    m_fbo->bind();
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);
    m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_shadowmapsDepth);
    m_fbo->unbind();

    m_blurHFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_blurTexture);
    m_blurVFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);
}

ShadowingStage::~ShadowingStage()
{
    delete m_blurHProgram;
    delete m_blurVProgram;
}

void ShadowingStage::render()
{
    glm::vec4 foo, bar;
    calculateSplitPlanes(1.f, 20.f, 4, 0.5f, foo, bar);

    AxisAlignedBoundingBox bb = m_painter.scene()->boundingBox();

    Projection *projection1 = new Projection(Projection::PERSPECTIVE);
    Projection *projection2 = new Projection(Projection::PERSPECTIVE);

    MonoCamera lightCamera = MonoCamera("light1", projection1);
    lightCamera.setFovy(90.0);
    lightCamera.setZNear(m_painter.camera()->zNear());
    lightCamera.setZFar(m_painter.camera()->zFar());
    lightCamera.setViewport(glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    lightCamera.setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f) + bb.center(),
        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
    lightCamera.invalidate();
    
    MonoCamera light2Camera = MonoCamera("light2", projection2);
    light2Camera.setFovy(90.0);
    light2Camera.setZNear(m_painter.camera()->zNear());
    light2Camera.setZFar(m_painter.camera()->zFar());
    light2Camera.setViewport(glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    light2Camera.setView(glm::lookAt(glm::vec3(-2.0f, 5.5f, -4.0f) + bb.center(),
        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
    light2Camera.invalidate();

    glm::mat4x4 *biasLightViewProjection = new glm::mat4x4[SHADOWMAP_COUNT];
    biasLightViewProjection[0] = biasMatrix * lightCamera.viewProjection();
    biasLightViewProjection[1] = biasMatrix * light2Camera.viewProjection();
    
    glm::mat4x4 *lightViewProjection = new glm::mat4x4[SHADOWMAP_COUNT];
    lightViewProjection[0] = lightCamera.viewProjection();
    lightViewProjection[1] = light2Camera.viewProjection();
    
    m_program->setUniform("lightTransforms", SHADOWMAP_COUNT, lightViewProjection[0]);
    m_program->setUniform("inverseViewProjection", glm::inverse(m_painter.camera()->viewProjection()));

    m_painter.camera()->setUniformsIn(*m_program);

    m_fbo->clear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    m_fbo->bind();
    drawScene(m_painter.camera()->viewProjection(), m_program);
    m_fbo->unbind();
    
    //ScreenQuad screenQuad;
    //
    //m_shadowmaps->bindActive(gl::GL_TEXTURE0);
    //m_blurHProgram->setUniform("source", 0);
    //m_blurHProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    //screenQuad.draw(*m_blurHProgram, m_blurHFBO);
    //
    //m_blurTexture->bindActive(gl::GL_TEXTURE0);
    //m_blurVProgram->setUniform("source", 0);
    //m_blurVProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    //screenQuad.draw(*m_blurVProgram, m_blurVFBO);

    delete[] biasLightViewProjection;
    delete[] lightViewProjection;
}

void ShadowingStage::resize(const int width, const int height)
{

}

void ShadowingStage::reloadShaders()
{

}

void ShadowingStage::calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda, glm::vec4 &nearSplits, glm::vec4 &farSplits)
{
    int splits = glm::max(1, glm::min(planesCount, LAYER_COUNT));
    float nearResults[4] = { 0.f, 0.f, 0.f, 0.f };
    float farResults[4] = { 0.f, 0.f, 0.f, 0.f };

    for (int i = 0; i < splits; ++i)
    {
        nearResults[i] = lambda * (znear * pow(zfar / znear, static_cast<float>(i) / planesCount)) + (1.f - lambda) * (znear + (zfar - znear) * (static_cast<float>(i) / planesCount));
        farResults[i] = lambda * (znear * pow(zfar / znear, (static_cast<float>(i) + 1.f) / planesCount)) + (1.f - lambda) * (znear + (zfar - znear) * ((static_cast<float>(i) + 1.f) / planesCount));
    }

    nearSplits = glm::make_vec4(nearResults);
    farSplits = glm::make_vec4(farResults);
}

//void ShadowingStage::calculateCropMatrices()
//{
//
//}

//void VarianceShadowMappingPass::render()
//{
//    AxisAlignedBoundingBox bb = m_camera->asGroup()->boundingBox();
//
//    Camera lightCamera = Camera();
//    lightCamera.setFovy(90.0);
//    lightCamera.setZNear(m_camera->zNear());
//    lightCamera.setZFar(m_camera->zFar());
//    lightCamera.setViewport(glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
//    lightCamera.setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f) + bb.center(),
//        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
//    lightCamera.update();
//
//    Camera light2Camera = Camera();
//    light2Camera.setFovy(90.0);
//    light2Camera.setZNear(m_camera->zNear());
//    light2Camera.setZFar(m_camera->zFar());
//    light2Camera.setViewport(glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
//    light2Camera.setView(glm::lookAt(glm::vec3(-2.0f, 5.5f, -4.0f) + bb.center(),
//        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
//    light2Camera.update();
//
//	glm::mat4x4 *biasLightViewProjection = new glm::mat4x4[2];
//	biasLightViewProjection[0] = biasMatrix * lightCamera.transform();
//	biasLightViewProjection[1] = biasMatrix * light2Camera.transform();
//
//    glm::mat4x4 *lightViewProjection = new glm::mat4x4[2];
//    lightViewProjection[0] = lightCamera.transform();
//    lightViewProjection[1] = light2Camera.transform();
//
//    m_lightProgram->setUniform("lightTransforms", 2, lightViewProjection[0]);
//    m_lightProgram->setUniform("inverseViewProjection", m_camera->transformInverse());
//    drawScene(m_camera, m_lightProgram, m_shadowmapFBO3D);
//
//	ScreenQuad screenQuad;
//	m_shadowmapFBO3D->bindTexture3D(*m_blurv, "source", 0);
//	m_blurv->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
//	screenQuad.draw(*m_blurv, m_blurVFBO);
//
//	m_blurVFBO->bindTexture3D(*m_blurh, "source", 0);
//	m_blurh->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
//	screenQuad.draw(*m_blurh, m_shadowmapFBO3D);
//
//	m_shadowmapFBO3D->bindTexture3D(*m_program, "shadowmap3D", 0);
//	m_program->setUniform("inverseViewProjection", m_camera->transformInverse());
//	m_program->setUniform("biasLightViewProjection", 2, biasLightViewProjection[0]);
//	drawScene(m_camera, m_program, m_fbo);
//
//    delete[] biasLightViewProjection;
//    delete[] lightViewProjection;
//}