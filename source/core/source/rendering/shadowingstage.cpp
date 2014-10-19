#include <core/rendering/shadowingstage.h>

#include <cmath>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/globjects.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/base/File.h>

#include <core/camera/abstractcamera.h>
#include <core/camera/monocamera.h>
#include <core/camera/projection.h>
#include <core/painter/pipelinepainter.h>
#include <core/screenquad.h>
#include <core/viewfrustum.h>


namespace
{
    const int SHADOWMAP_SIZE = 512;
    const int LAYER_COUNT = 4;
    const int SHADOWMAP_COUNT = 8;
    const float PI = 3.14159265358979323846f;

    const glm::mat4 BIAS_MATRIX(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );

    std::array<glm::vec3, 8> calculatePointsOfViewFrustum(AbstractCamera *camera)
    {
   		std::array<glm::vec3, 8> result;
   		glm::vec3 up = glm::normalize(camera->up());
   		glm::vec3 eye = camera->eye();
   		glm::vec3 direction = glm::normalize(camera->center() - eye);
   		float zFar = camera->zFar();
   		float zNear = camera->zNear();
   
   		glm::vec3 right = glm::normalize(glm::cross(direction, up));
   
   		glm::vec3 farPlaneCenter = eye + direction * zFar;
   		glm::vec3 nearPlaneCenter = eye + direction * zNear;
   
   		float nearHeight = std::tan(camera->fovy() * PI / 360.f) * camera->zNear();
   		float nearWidth = nearHeight * camera->aspect();
   		float farHeight = std::tan(camera->fovy() * PI / 360.f) * camera->zFar();
   		float farWidth = farHeight * camera->aspect();
   
   		result[0]=(nearPlaneCenter - up * nearHeight - right * nearWidth);
   		result[1]=(nearPlaneCenter + up * nearHeight - right * nearWidth);
   		result[2]=(nearPlaneCenter + up * nearHeight + right * nearWidth);
   		result[3]=(nearPlaneCenter - up * nearHeight + right * nearWidth);
   		result[4]=(farPlaneCenter - up * farHeight - right * farWidth);
   		result[5]=(farPlaneCenter + up * farHeight - right * farWidth);
   		result[6]=(farPlaneCenter + up * farHeight + right * farWidth);
   		result[7]=(farPlaneCenter - up * farHeight + right * farWidth);
   
   		return result;
    };
}

ShadowingStage::ShadowingStage(PipelinePainter &painter)
:   AbstractSceneRenderStage(painter)
,   m_shadowmaps(globjects::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_shadowmapsDepth(globjects::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurTexture(globjects::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY))
,   m_blurHProgram(new globjects::Program())
,   m_blurVProgram(new globjects::Program())
,   m_blurHFBO(new globjects::Framebuffer())
,   m_blurVFBO(new globjects::Framebuffer())
//following members are only needed for visualisation of shadows, because no ligthingsystem exists
, m_testFBO(new globjects::Framebuffer())
, m_testTexture(globjects::Texture::createDefault())
, m_testTextureDepth(globjects::Texture::createDefault())
, m_testProgram(new globjects::Program())
{    
    m_painter.addTexture("shadowmaps", m_shadowmaps);

    m_program->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/shadows/shadowmap_creation.vert")));
    m_program->attach(new globjects::Shader(gl::GL_GEOMETRY_SHADER, new globjects::File("data/shadows/shadowmap_creation.geom")));
    m_program->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/shadows/shadowmap_creation.frag")));

    m_blurHProgram->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/screenquad.vert")));
    m_blurHProgram->attach(new globjects::Shader(gl::GL_GEOMETRY_SHADER, new globjects::File("data/shadows/screenquad_layered.geom")));
    m_blurHProgram->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/shadows/gauss_blur_5_h_layered.frag")));
    
    m_blurVProgram->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/screenquad.vert")));
    m_blurVProgram->attach(new globjects::Shader(gl::GL_GEOMETRY_SHADER, new globjects::File("data/shadows/screenquad_layered.geom")));
    m_blurVProgram->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/shadows/gauss_blur_5_v_layered.frag")));

    m_shadowmaps->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_shadowmapsDepth->image3D(0, gl::GL_DEPTH_COMPONENT32, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_INT, nullptr);
    m_blurTexture->image3D(0, gl::GL_RGBA32F, glm::ivec3(SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_COUNT), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);    

    m_fbo->bind(gl::GL_FRAMEBUFFER);
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);
    m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_shadowmapsDepth);
    m_fbo->unbind(gl::GL_FRAMEBUFFER);

    m_blurHFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_blurTexture);
    m_blurVFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_shadowmaps);

    //visualisation begin
    m_painter.addTexture("lighting", m_testTexture);
    m_testProgram->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/shadows/visualisation.frag")));
    m_testProgram->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/shadows/shadowing.glsl")));
    m_testProgram->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/shadows/visualisation.vert")));

    m_testFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_testTexture);
    m_testFBO->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_testTextureDepth);
    //visualisation end

    resize(painter.camera()->viewport().x, painter.camera()->viewport().y);
}

ShadowingStage::~ShadowingStage()
{
    m_painter.removeTexture("shadowmaps");
}

void ShadowingStage::removeAllLights()
{
    m_lightViewProjections.clear();
    m_lightBiasedViewProjections.clear();
}

int ShadowingStage::addDirectionalLight(glm::vec3 direction)
{
    if (m_lightViewProjections.size() > SHADOWMAP_COUNT - LAYER_COUNT) return -1;
    int startIndex = m_lightViewProjections.size();
    m_splitPlanes = calculateSplitPlanes(m_painter.camera()->zNear(), m_painter.camera()->zFar(), LAYER_COUNT, .5f);

    MonoCamera lightCamera = MonoCamera("light", nullptr);
    lightCamera.setView(glm::lookAt(glm::vec3(0.f, 0.f, 0.f), direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    lightCamera.invalidate();

    AxisAlignedBoundingBox bb = m_painter.scene()->boundingBox();
    bb *= lightCamera.view();
    
    glm::vec3 minValues = glm::min(bb.urb(), bb.llf());
    glm::vec3 maxValues = glm::max(bb.urb(), bb.llf());

    float scaleX, scaleY, scaleZ;
    float translateX, translateY, translateZ;

    scaleX = 2.0f / (maxValues.x - minValues.x);
    scaleY = 2.0f / (maxValues.y - minValues.y);
    translateX = -0.5f * (maxValues.x + minValues.x) * scaleX;
    translateY = -0.5f * (maxValues.y + minValues.y) * scaleY;
    scaleZ = 1.0f / (-maxValues.z + minValues.z);
    translateZ = -minValues.z * scaleZ;
    
    glm::mat4 projection(
        scaleX, 0.f, 0.f, 0.f,
        0.f, scaleY, 0.f, 0.f,
        0.f, 0.f, scaleZ, 0.f,
        translateX, translateY, translateZ, 1.f);

    glm::mat4 viewProjection = projection * lightCamera.view();

    std::vector<glm::mat4> cropMatrices = calculateCropMatrices(*m_painter.camera(), viewProjection, m_splitPlanes);
    for (int i = 0; i < cropMatrices.size(); ++i)
    {
        glm::mat4 lightCroppedViewProjection = cropMatrices[i] * viewProjection;
        m_lightViewProjections.push_back(lightCroppedViewProjection);
        m_lightBiasedViewProjections.push_back(BIAS_MATRIX * lightCroppedViewProjection);
    }
    return startIndex;
}

int ShadowingStage::addSpotLight(glm::vec3 position, glm::vec3 direction, float fovy)
{
    if (m_lightViewProjections.size() > SHADOWMAP_COUNT - LAYER_COUNT) return -1;

    int startIndex = m_lightViewProjections.size();
    m_splitPlanes = calculateSplitPlanes(m_painter.camera()->zNear(), m_painter.camera()->zFar(), LAYER_COUNT, .5f);

    Projection *projection = new Projection(Projection::PERSPECTIVE);
    MonoCamera lightCamera = MonoCamera("light", projection);
    lightCamera.setFovy(fovy);
    lightCamera.setZNear(1.0f);
    lightCamera.setZFar(20.0f);
    lightCamera.setView(glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    lightCamera.invalidate();

    std::vector<glm::mat4> cropMatrices = calculateCropMatrices(*m_painter.camera(), lightCamera.viewProjection(), m_splitPlanes);
    for (int i = 0; i < cropMatrices.size(); ++i)
    {
        glm::mat4 lightCroppedViewProjection = cropMatrices[i] * lightCamera.viewProjection();
        m_lightViewProjections.push_back(lightCroppedViewProjection);
        m_lightBiasedViewProjections.push_back(BIAS_MATRIX * lightCroppedViewProjection);
    }
    return startIndex;
}

void ShadowingStage::smoothShadowmaps()
{
    ScreenQuad screenQuad;
    m_shadowmaps->bindActive(gl::GL_TEXTURE0);
    m_blurHProgram->setUniform("source", 0);
    m_blurHProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    screenQuad.draw(*m_blurHProgram, m_blurHFBO);

    m_blurTexture->bindActive(gl::GL_TEXTURE0);
    m_blurVProgram->setUniform("source", 0);
    m_blurVProgram->setUniform("viewport", glm::ivec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
    screenQuad.draw(*m_blurVProgram, m_blurVFBO);
}

void ShadowingStage::render()
{
    //visualisation begin
    removeAllLights();
    addSpotLight(glm::vec3(4.0f, 5.5f, -6.0f), glm::vec3(-4.0f, -5.5f, 6.0f), 45.f);
    addDirectionalLight(glm::vec3(1.f, -1.f, 1.f));
    //visualisation end
    
    m_lightViewProjections.shrink_to_fit();
    m_program->setUniform("lightTransforms", m_lightViewProjections);

    m_painter.setCameraUniforms(*m_program);

    gl::glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
    m_fbo->clear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    m_fbo->bind(gl::GL_FRAMEBUFFER);
    drawScene(m_painter.camera()->viewProjection(), m_program);
    m_fbo->unbind(gl::GL_FRAMEBUFFER);
    
    smoothShadowmaps();
    gl::glViewport(0, 0, m_painter.camera()->viewport().x, m_painter.camera()->viewport().y);

    //visualisation begin
    m_shadowmaps->bindActive(gl::GL_TEXTURE0);
    m_testProgram->setUniform("shadowmaps", 0);
    m_testProgram->setUniform("lightBiasedViewProjections", getBiasedViewProjections());
    m_testProgram->setUniform("farSplits", getFarSplits());
    m_painter.setCameraUniforms(*m_testProgram);
    m_testFBO->clear(gl::GL_DEPTH_BUFFER_BIT | gl::GL_COLOR_BUFFER_BIT);
    m_testFBO->bind(gl::GL_FRAMEBUFFER);
    drawScene(m_painter.camera()->viewProjection(), m_testProgram);
    m_testFBO->unbind(gl::GL_FRAMEBUFFER);
    //visualisation end
}

void ShadowingStage::resize(const int width, const int height)
{
    //visualisation begin
    m_testTexture->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_testTextureDepth->image2D(0, gl::GL_DEPTH_COMPONENT32, width, height, 0, gl::GL_DEPTH_COMPONENT, gl::GL_FLOAT, nullptr);
    //visualisation end
}

void ShadowingStage::reloadShaders()
{

}

std::vector<glm::vec2> ShadowingStage::calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda)
{
    int splitsCount = glm::max(1, glm::min(planesCount, LAYER_COUNT));
    std::vector<glm::vec2> result;

    for (int i = 0; i < splitsCount; ++i)
    {
        glm::vec2 split;
        split.x = lambda * (znear * pow(zfar / znear, static_cast<float>(i) / splitsCount)) + (1.f - lambda) * (znear + (zfar - znear) * (static_cast<float>(i) / splitsCount));
        split.y = lambda * (znear * pow(zfar / znear, (static_cast<float>(i)+1.f) / splitsCount)) + (1.f - lambda) * (znear + (zfar - znear) * ((static_cast<float>(i)+1.f) / splitsCount));
        result.push_back(split);
    }

    result.resize(splitsCount, glm::vec2(1.0, 1.0));
    return result;
}

std::vector<glm::mat4> ShadowingStage::calculateCropMatrices(AbstractCamera &camera, glm::mat4 lightTransform, std::vector<glm::vec2> splits)
{
    std::vector<glm::mat4> result;
    int splitsCount = glm::max(1, glm::min(static_cast<int>(splits.size()), LAYER_COUNT));

    float zNear = camera.zNear();
    float zFar = camera.zFar();

    for (int i = 0; i < splitsCount; ++i)
    {
        camera.setZNear(splits[i].x);
        camera.setZFar(splits[i].y);

        std::array<glm::vec3, 8> points = calculatePointsOfViewFrustum(&camera);

        std::array<glm::vec3, 8>::const_iterator pointsBegin = points.begin();
        std::array<glm::vec3, 8>::const_iterator pointsEnd = points.end();

		glm::vec4 firstVertex = lightTransform * glm::vec4(*pointsBegin, 1.0);
        glm::vec3 minValues = (firstVertex / firstVertex.w).xyz;
        glm::vec3 maxValues = minValues;

		for (++pointsBegin; pointsBegin != pointsEnd; ++pointsBegin)
        {
            glm::vec4 vertex = lightTransform * glm::vec4(*pointsBegin, 1.0);
            vertex /= vertex.w;
            minValues = glm::min(minValues, glm::vec3(vertex.xyz));
            maxValues = glm::max(maxValues, glm::vec3(vertex.xyz));
        }

		minValues = glm::clamp(minValues, glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.f, 1.f, 1.f));
		maxValues = glm::clamp(maxValues, glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.f, 1.f, 1.f));
		glm::mat4x4 cropMatrix = glm::ortho(minValues.x, maxValues.x, minValues.y, maxValues.y, 1.f, -1.f);
		result.push_back(cropMatrix);
	}
	camera.setZFar(zFar);
    camera.setZNear(zNear);
	return result;
}

std::vector<glm::mat4> ShadowingStage::getBiasedViewProjections()
{
    m_lightBiasedViewProjections.shrink_to_fit();
    return m_lightBiasedViewProjections;
}

std::vector<float> ShadowingStage::getFarSplits()
{
    std::vector<float> result;
    for (int i = 0; i < m_splitPlanes.size(); ++i)
        result.push_back(m_splitPlanes[i].y);
    return result;
}