#pragma once

#include <core/core_api.h>
#include <core/rendering/renderstage.h>

// under construction
class CORE_API SceneRenderStage : public RenderStage
{
public:
    SceneRenderStage(PipelinePainter & painter);
    virtual ~SceneRenderStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
    virtual void render() override;

protected:
	Program * m_program;
	FrameBufferObject * m_fbo;

};
