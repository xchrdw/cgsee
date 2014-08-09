#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>

class RenderBufferObject;

namespace glow{
    class Texture;
}

class CORE_API RenderStage : public AbstractSceneRenderStage
{
public:
    RenderStage(PipelinePainter & painter);
    virtual ~RenderStage(void);

    virtual void render() override;

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;

protected:
    glow::Texture * m_normalz;
    glow::Texture * m_colorId;
    RenderBufferObject * m_depth;
};
