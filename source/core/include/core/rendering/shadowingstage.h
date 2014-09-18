#pragma once

#include <core/core_api.h>

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/rendering/abstractscenerenderstage.h>

class AbstractCamera;

namespace glo{
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
    virtual std::vector<glm::mat4> getBiasedViewProjections();

    virtual void removeAllLights();
    virtual void addSpotLight(glm::vec3 position, glm::vec3 direction, float fovy);

protected:
    std::vector<glm::vec2> calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda);
    std::vector<glm::mat4> calculateCropMatrices(AbstractCamera &camera, glm::mat4 lightTransform, std::vector<glm::vec2> splits);

protected:
    glo::Texture *m_shadowmaps;
    glo::Texture *m_shadowmapsDepth;
    glo::Texture *m_blurTexture;
    glo::Program *m_blurHProgram;
    glo::Program *m_blurVProgram;
    glo::FrameBufferObject *m_blurHFBO;
    glo::FrameBufferObject *m_blurVFBO;

    glo::Program *m_testProgram;
    glo::Texture *m_testTexture;
    glo::Texture *m_testTextureDepth;
    glo::FrameBufferObject *m_testFBO;

    std::vector<glm::mat4> m_lightViewProjections;
    std::vector<glm::mat4> m_lightBiasedViewProjections;
};