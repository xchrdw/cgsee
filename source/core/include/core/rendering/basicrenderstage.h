#pragma once

#include <core/core_api.h>
#include <core/rendering/scenerenderstage.h>

// under construction
class CORE_API BasicRenderStage : public SceneRenderStage
{
public:
    BasicRenderStage(PipelinePainter & painter);
    virtual ~BasicRenderStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
};
