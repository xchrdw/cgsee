#pragma once

#include <core/core_api.h>

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/rendering/abstractscenerenderstage.h>

class AbstractCamera;

namespace globjects{
    class FrameBufferObject;
    class Program;
    class Texture;    
}

class CORE_API ShadowingStage : public AbstractSceneRenderStage
{
public:
    ShadowingStage(PipelinePainter & painter);
    virtual ~ShadowingStage(void);

    virtual void render() override;
    virtual void resize(const int width, const int height) override;
    virtual void reloadShaders() override;

    virtual void removeAllLights();
    virtual int addSpotLight(glm::vec3 position, glm::vec3 direction, float fovy);
    virtual int addDirectionalLight(glm::vec3 direction);    
    
    virtual std::vector<glm::mat4> getBiasedViewProjections();
    virtual std::vector<float> getFarSplits();

protected:
    std::vector<glm::vec2> calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda);
    std::vector<glm::mat4> calculateCropMatrices(AbstractCamera &camera, glm::mat4 lightTransform, std::vector<glm::vec2> splits);

    virtual void smoothShadowmaps();

protected:
    globjects::Texture *m_shadowmaps;
    globjects::Texture *m_shadowmapsDepth;
    globjects::Texture *m_blurTexture;
    globjects::Program *m_blurHProgram;
    globjects::Program *m_blurVProgram;
    globjects::Framebuffer *m_blurHFBO;
    globjects::Framebuffer *m_blurVFBO;

    globjects::Program *m_testProgram;
    globjects::Texture *m_testTexture;
    globjects::Texture *m_testTextureDepth;
    globjects::Framebuffer *m_testFBO;

    std::vector<glm::mat4> m_lightViewProjections;
    std::vector<glm::mat4> m_lightBiasedViewProjections;
    std::vector<glm::vec2> m_splitPlanes;
};