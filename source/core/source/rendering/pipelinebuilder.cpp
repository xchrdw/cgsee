#include <core/rendering/pipelinebuilder.h>
#include <core/camera/monocamera.h>
#include <core/camera/stereocamera.h>

PipelineBuilder::PipelineBuilder(PipelinePainter & painter)
    : m_painter(painter)
    , m_camera(painter.camera())
{

}

PipelineBuilder::~PipelineBuilder()
{

}

bool PipelineBuilder::build()
{
    if (m_camera->isStereo())
    {
        if (!buildPipeline(m_camera->leftCamera()))
            return false;
        if (!buildPipeline(m_camera->rightCamera()))
            return false;
        if (!addJoinStage())
            return false;
    }
    else
    {
        if (!buildPipeline(m_camera))
            return false;
    }

    return true;
}

bool PipelineBuilder::buildPipeline(MonoCameraNew * camera)
{
    if (!addColorIdStage(camera))
        return false;
    if (!addRenderingStages(camera))
        return false;
    if (!addPostProcessingStages(camera))
        return false;
    return true;
}

bool PipelineBuilder::addJoinStage()
{

    return true;
}

bool PipelineBuilder::addColorIdStage(MonoCameraNew * camera)
{

    return true;
}
