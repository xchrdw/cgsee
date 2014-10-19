#pragma once

#include <core/core_api.h>

#include <globjects/Program.h>
#include <globjects/base/ref_ptr.h>

#include <core/rendering/abstractrenderstage.h>

class Screenquad;


// under construction
class CORE_API AbstractPostProcessingStage : public AbstractRenderStage
{
public:
    AbstractPostProcessingStage(PipelinePainter & painter);
    virtual ~AbstractPostProcessingStage(void);

    virtual void render() override;

protected:
    ScreenQuad * m_screenquad;
	globjects::ref_ptr<globjects::Program> m_program;
};
