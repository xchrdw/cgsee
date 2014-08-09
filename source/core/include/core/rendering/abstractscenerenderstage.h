#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractrenderstage.h>

class Program;
class PipelinePainter;

// under construction
class CORE_API AbstractSceneRenderStage : public AbstractRenderStage
{
public:
    AbstractSceneRenderStage(PipelinePainter & painter);
    virtual ~AbstractSceneRenderStage(void);

    virtual void render() override;

protected:
    Program * m_program;
};
