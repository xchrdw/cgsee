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
    virtual bool buildPipeline(const MonoCameraNew * camera);
    virtual bool addJoinStage();

    virtual bool addColorIdStage(const MonoCameraNew * camera);
    virtual bool addRenderingStages(const MonoCameraNew * camera) = 0;
    virtual bool addPostProcessingStages(const MonoCameraNew * camera) = 0;

protected:
    PipelinePainter & m_painter;
    AbstractCamera * m_camera;
};
