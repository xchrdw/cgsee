#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>

class RenderBufferObject;

namespace globjects{
    class Texture;
    class Renderbuffer;
}

class CORE_API RenderStage : public AbstractSceneRenderStage
{
public:
    RenderStage(PipelinePainter & painter, const QString & normalzBufferName);
    virtual ~RenderStage(void);

    virtual void render() override;

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;

protected:
    globjects::Texture * m_normalz;
    globjects::Texture * m_colorId;
    globjects::Renderbuffer * m_depth;
};
