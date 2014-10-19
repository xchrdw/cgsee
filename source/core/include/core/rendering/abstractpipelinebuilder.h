#pragma once

#include <core/painter/pipelinepainter.h>

class AbstractCamera;
class MonoCamera;

class AbstractPipelineBuilder
{
public:
    AbstractPipelineBuilder(PipelinePainter & painter);
    virtual ~AbstractPipelineBuilder();

    virtual bool build();

protected:
    virtual bool buildPipeline(const MonoCamera * camera) = 0;
    virtual bool addJoinStage();

    virtual bool addColorIdStage(const MonoCamera * camera);
protected:
    PipelinePainter & m_painter;
    AbstractCamera * m_camera;
};
