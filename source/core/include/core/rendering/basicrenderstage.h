#pragma once

#include <core/core_api.h>
#include <core/rendering/scenerenderstage.h>
#include <core/textureobject.h>
#include <core/renderbufferobject.h>

class CORE_API BasicRenderStage : public SceneRenderStage
{
public:
    BasicRenderStage(PipelinePainter & painter);
    virtual ~BasicRenderStage(void);

    virtual void render() override;

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;

protected:
    TextureObject * m_normalz;
    TextureObject * m_colorId;
    RenderBufferObject * m_depth;
};
