#include <core/rendering/shadowingstage.h>

#include <cmath>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/globjects.h>
#include <globjects/FrameBufferObject.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects-base/File.h>

#include <core/camera/abstractcamera.h>
#include <core/camera/monocamera.h>
#include <core/camera/projection.h>
#include <core/painter/pipelinepainter.h>
#include <core/screenquad.h>


namespace
{
    const int SHADOWMAP_SIZE = 512;
    const int LAYER_COUNT = 4;
    const int SHADOWMAP_COUNT = 2;
    const float PI = 3.14159265358979323846f;

    const glm::mat4 BIAS_MATRIX(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );
}

ShadowingStage::ShadowingStage(PipelinePainter &painter)
:   AbstractSceneRenderStage(painter)
,   m_shadowmaps(glo::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_shadowmapsDepth(glo::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurTexture(glo::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurHProgram(new glo::Program())
,   m_blurVProgram(new glo::Program())
,   m_blurHFBO(new glo::FrameBufferObject())
,   m_blurVFBO(new glo::FrameBufferObject())
, m_testFBO(new glo::FrameBufferObject())
, m_testTexture(glo::Texture::createDefault())
, m_testTextureDepth(glo::Texture::createDefault())
, m_testProgram(new glo::Program())
{    
    m_painter.addTexture("shadowmaps", m_shadowmaps);
    m_painter.addTexture("lighting", m_testTexture);

    m_program->attach(new glo::Shader(gl::GL_VERTEX_SHADER, new glo::File("data/shadows/vsm_light.vert")));
    m_program->attach(new glo::Shader(gl::GL_GEOMETRY_SHADER, new glo::File("data/shadows/vsm_light.geom")));
    m_program->attach(new glo::Shader(gl::GL_FRAGMENT_SHADER, new glo::File("data/shadows/vsm_light.frag")));

    m_blurHProgram->attach(new glo::Shader(gl::GL_VERTEX_SHADER, new glo::File("data/screenquad.vert")));
    m_blurHProgram->attach(new glo::Shader(gl::GL_GEOMETRY_SHADER, new glo::File("data/shadows/screenquad_layered.geom")));
    m_blurHProgram->attach(new glo::Shader(gl::GL_FRAGMENT_SHADER, new glo::File("data/shadows/gauss_blur_5_h_layered.frag")));
    
    m_blurVProgram->attach(new glo::Shader(gl::GL_VERTEX_SHADER, new glo::File("data/screenquad.vert")));
    m_blurVProgram->attach(new glo::Shader(gl::GL_GEOMETRY_SHADER, new glo::File("data/shadows/screenquad_layered.geom")));
    m_blurVProgram->attach(new glo::Shader(gl::GL_FRAGMENT_SHADER, new glo::File("data/shadows/gauss_blur_5_v_layered.frag")));

    m_shadowmaps->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_shadowmapsDepth->image3D(0, gl::GL_DEPTH_COMPONENT32, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_INT, nullptr);
    m_blurTexture->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);    

    m_fbo->bind();
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);
    m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_shadowmapsDepth);
    m_fbo->unbind();

    m_blurHFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_blurTexture);
    m_blurVFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);

    m_testProgram->attach(new glo::Shader(gl::GL_FRAGMENT_SHADER, new glo::File("data/test.frag")));
    m_testProgram->attach(new glo::Shader(gl::GL_FRAGMENT_SHADER, new glo::File("data/shadows/shadowing.glsl")));
    m_testProgram->attach(new glo::Shader(gl::GL_VERTEX_SHADER, new glo::File("data/test.vert")));

    m_testFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_testTexture);
    m_testFBO->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_testTextureDepth);    

    resize(painter.camera()->viewport().x, painter.camera()->viewport().y);
}

ShadowingStage::~ShadowingStage()
{
    m_painter.removeTexture("shadowmaps");
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

    m_biasedViewProjections.clear();
    m_biasedViewProjections.push_back(BIAS_MATRIX * lightCamera.viewProjection());
    m_biasedViewProjections.push_back(BIAS_MATRIX * light2Camera.viewProjection());
    m_biasedViewProjections.shrink_to_fit();
    
    std::array<glm::mat4x4, SHADOWMAP_COUNT> lightViewProjection;
    lightViewProjection[0] = lightCamera.viewProjection();
    lightViewProjection[1] = light2Camera.viewProjection();
    
    m_program->setUniform("lightTransforms", lightViewProjection);

    m_painter.camera()->setUniformsIn(*m_program);

    gl::glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
    m_fbo->clear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    m_fbo->bind();
    drawScene(m_painter.camera()->viewProjection(), m_program);
    m_fbo->unbind();
    
    ScreenQuad screenQuad;
    m_shadowmaps->bindActive(gl::GL_TEXTURE0);
    m_blurHProgram->setUniform("source", 0);
    m_blurHProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    screenQuad.draw(*m_blurHProgram, m_blurHFBO);
    
    m_blurTexture->bindActive(gl::GL_TEXTURE0);
    m_blurVProgram->setUniform("source", 0);
    m_blurVProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    screenQuad.draw(*m_blurVProgram, m_blurVFBO);
    gl::glViewport(0, 0, m_painter.camera()->viewport().x, m_painter.camera()->viewport().y);


    //visualization
    m_shadowmaps->bindActive(gl::GL_TEXTURE0);
    m_testProgram->setUniform("shadowmaps", 0);
    m_testProgram->setUniform("lightBiasedViewProjections", getBiasedViewProjections());
    m_painter.camera()->setUniformsIn(*m_testProgram);
    m_testFBO->clear(gl::GL_DEPTH_BUFFER_BIT | gl::GL_COLOR_BUFFER_BIT);
    m_testFBO->bind();
    drawScene(m_painter.camera()->viewProjection(), m_testProgram);
    m_testFBO->unbind();
}

void ShadowingStage::resize(const int width, const int height)
{
    m_testTexture->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_testTextureDepth->image2D(0, gl::GL_DEPTH_COMPONENT32, width, height, 0, gl::GL_DEPTH_COMPONENT, gl::GL_FLOAT, nullptr);
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

std::vector<glm::mat4> ShadowingStage::getBiasedViewProjections()
{
    return m_biasedViewProjections;
}

//void ShadowingStage::calculateCropMatrices()
//{
//
//}