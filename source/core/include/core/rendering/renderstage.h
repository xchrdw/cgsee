#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>

class RenderBufferObject;

namespace glo{
    class Texture;
    class RenderBufferObject;
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
    glo::Texture * m_normalz;
    glo::Texture * m_colorId;
    glo::RenderBufferObject * m_depth;
};
