#pragma once

#include <core/painter/pipelinepainter.h>
#include <core/rendering/abstractpipelinebuilder.h>


class PathtracingPipelineBuilder : public AbstractPipelineBuilder
{
public:
    PathtracingPipelineBuilder(PipelinePainter & painter);
    virtual ~PathtracingPipelineBuilder();


protected:
};
