#pragma once

#include <core/core_api.h>

#include <globjects/Program.h>
#include <globjects/base/ref_ptr.h>

#include <core/rendering/abstractrenderstage.h>

class PipelinePainter;

// under construction
class CORE_API AbstractSceneRenderStage : public AbstractRenderStage
{
public:
    AbstractSceneRenderStage(PipelinePainter & painter);
    virtual ~AbstractSceneRenderStage(void);

    virtual void render() override;

protected:
    globjects::ref_ptr<globjects::Program> m_program;
};
