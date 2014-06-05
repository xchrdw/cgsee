#pragma once

#include <core/core_api.h>
#include <core/rendering/renderstage.h>


// under construction
class CORE_API PostProcessingStage : public RenderStage
{
public:
    PostProcessingStage(PipelinePainter & painter);
    virtual ~PostProcessingStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
    virtual void render() ;

protected:
	Program * m_program;
};
