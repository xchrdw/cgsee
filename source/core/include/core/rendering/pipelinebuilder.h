#pragma once

#include <core/painter/pipelinepainter.h>

class AbstractCamera;
class MonoCameraNew;

class PipelineBuilder
{
public:
    PipelineBuilder(PipelinePainter & painter);
    virtual ~PipelineBuilder();

    virtual bool build();

protected:
    virtual bool buildPipeline(MonoCameraNew * camera);
    virtual bool addJoinStage();

    virtual bool addColorIdStage(MonoCameraNew * camera);
    virtual bool addRenderingStages(MonoCameraNew * camera) = 0;
    virtual bool addPostProcessingStages(MonoCameraNew * camera) = 0;

protected:
    PipelinePainter & m_painter;
    AbstractCamera * m_camera;
};
