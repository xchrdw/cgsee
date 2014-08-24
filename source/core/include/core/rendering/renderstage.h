#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractscenerenderstage.h>
#include <core/textureobject.h>
#include <core/renderbufferobject.h>

class CORE_API RenderStage : public AbstractSceneRenderStage
{
public:
    RenderStage(PipelinePainter & painter, const QString & normalzBufferName);
    virtual ~RenderStage(void);

    virtual void render() override;

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;

protected:
    TextureObject * m_normalz;
    TextureObject * m_colorId;
    RenderBufferObject * m_depth;
};
