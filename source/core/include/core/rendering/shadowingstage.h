#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>

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

protected:
    void calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda, glm::vec4 &nearSplits, glm::vec4 &farSplits);

protected:
    glo::Texture *m_shadowmaps;
    glo::Texture *m_shadowmapsDepth;
    glo::Texture *m_blurTexture;
    glo::Program *m_blurHProgram;
    glo::Program *m_blurVProgram;
    glo::FrameBufferObject *m_blurHFBO;
    glo::FrameBufferObject *m_blurVFBO;
};