#pragma once

#include <core/painter/pipelinepainter.h>
#include <core/rendering/abstractpipelinebuilder.h>


class RasterizationPipelineBuilder : public AbstractPipelineBuilder
{
public:
    RasterizationPipelineBuilder(PipelinePainter & painter);
    virtual ~RasterizationPipelineBuilder();

protected:
    virtual bool buildPipeline(const MonoCamera * camera) override;
};
