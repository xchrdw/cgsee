#pragma once

#include <core/core_api.h>

#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>
#include <globjects/base/ref_ptr.h>

#include <core/rendering/abstractscenerenderstage.h>

class CORE_API RenderStage : public AbstractSceneRenderStage
{
public:
    RenderStage(PipelinePainter & painter, const QString & normalzBufferName);
    virtual ~RenderStage(void);

    virtual void render() override;

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;

protected:
    globjects::ref_ptr<globjects::Texture> m_normalz;
    globjects::ref_ptr<globjects::Texture> m_colorId;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;
};
