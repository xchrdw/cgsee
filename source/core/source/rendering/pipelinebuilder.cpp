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
        StereoCamera * stereoCamera = dynamic_cast<StereoCamera*>(m_camera);
        assert(stereoCamera);
        if (!buildPipeline(stereoCamera->leftCamera()))
            return false;
        if (!buildPipeline(stereoCamera->rightCamera()))
            return false;
        if (!addJoinStage())
            return false;
    }
    else
    {
        MonoCamera * monoCamera = dynamic_cast<MonoCamera*>(m_camera);
        assert(monoCamera);
        if (!buildPipeline(monoCamera))
            return false;
    }

    return true;
}

bool PipelineBuilder::buildPipeline(const MonoCamera * camera)
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
    //TODO
    return true;
}

bool PipelineBuilder::addColorIdStage(const MonoCamera * camera)
{
    //TODO
    return true;
}
