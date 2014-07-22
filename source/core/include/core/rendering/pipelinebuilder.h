#pragma once

#include <core/painter/pipelinepainter.h>

class AbstractCamera;
class MonoCamera;

class PipelineBuilder
{
public:
    PipelineBuilder(PipelinePainter & painter);
    virtual ~PipelineBuilder();

    virtual bool build();

protected:
    virtual bool buildPipeline(const MonoCamera * camera);
    virtual bool addJoinStage();

    virtual bool addColorIdStage(const MonoCamera * camera);
    virtual bool addRenderingStages(const MonoCamera * camera) = 0;
    virtual bool addPostProcessingStages(const MonoCamera * camera) = 0;

protected:
    PipelinePainter & m_painter;
    AbstractCamera * m_camera;
};
