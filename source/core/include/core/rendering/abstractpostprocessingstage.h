#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractrenderstage.h>

class Program;
class PipelinePainter;

// under construction
class CORE_API AbstractPostProcessingStage : public AbstractRenderStage
{
public:
    AbstractPostProcessingStage(PipelinePainter & painter);
    virtual ~AbstractPostProcessingStage(void);

    virtual void render() override;

protected:
	Program * m_program;
};
