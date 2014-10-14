#pragma once

#include <core/core_api.h>
#include <core/rendering/abstractrenderstage.h>

class Screenquad;

namespace globjects{
    class Program;
}

// under construction
class CORE_API AbstractPostProcessingStage : public AbstractRenderStage
{
public:
    AbstractPostProcessingStage(PipelinePainter & painter);
    virtual ~AbstractPostProcessingStage(void);

    virtual void render() override;

protected:
    ScreenQuad * m_screenquad;
	globjects::Program * m_program;
};
