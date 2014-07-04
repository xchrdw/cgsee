#pragma once

#include <core/painter/pipelinepainter.h>
#include <core/rendering/pipelinebuilder.h>


class PathtracingPipelineBuilder : public PipelineBuilder
{
public:
    PathtracingPipelineBuilder(PipelinePainter & painter);
    virtual ~PathtracingPipelineBuilder();


protected:
};
