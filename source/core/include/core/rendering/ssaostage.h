#pragma once

#include <core/core_api.h>
#include <core/rendering/postprocessingstage.h>


// under construction
class CORE_API SsaoStage : public PostProcessingStage
{
public:
    SsaoStage(PipelinePainter & painter);
    virtual ~SsaoStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
};
