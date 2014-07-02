#pragma once

#include <core/painter/pipelinepainter.h>
#include <core/rendering/pipelinebuilder.h>


class RasterizationPipelineBuilder : public PipelineBuilder
{
public:
    RasterizationPipelineBuilder(PipelinePainter & painter);
    virtual ~RasterizationPipelineBuilder();

protected:
    virtual bool addRenderingStages(MonoCamera * camera) override;
    virtual bool addPostProcessingStages(MonoCamera * camera) override;


protected:
};
