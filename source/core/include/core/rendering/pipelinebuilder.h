#pragma once

#include <core/painter/pipelinepainter.h>


class PipelineBuilder
{
public:
    PipelineBuilder(PipelinePainter & painter);
    virtual ~PipelineBuilder();

    virtual void build(/*cam config*/) = 0;

protected:
    PipelinePainter & m_painter;
};
