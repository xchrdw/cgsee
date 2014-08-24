#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>
#include <core/program.h>

namespace glow{
    class Texture;
    class FrameBufferObject;
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
    glow::Texture *m_shadowmaps;
    glow::Texture *m_shadowmapsDepth;
    glow::Texture *m_blurTexture;
    Program *m_blurHProgram;
    Program *m_blurVProgram;
    glow::FrameBufferObject *m_blurHFBO;
    glow::FrameBufferObject *m_blurVFBO;
};