#include <core/rendering/abstractpipelinebuilder.h>

#include <core/camera/monocamera.h>
#include <core/camera/stereocamera.h>

AbstractPipelineBuilder::AbstractPipelineBuilder(PipelinePainter & painter)
    : m_painter(painter)
    , m_camera(painter.camera())
{

}

AbstractPipelineBuilder::~AbstractPipelineBuilder()
{

}

bool AbstractPipelineBuilder::build()
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

bool AbstractPipelineBuilder::addJoinStage()
{
    //TODO
    return true;
}

bool AbstractPipelineBuilder::addColorIdStage(const MonoCamera * camera)
{
    //TODO
    return true;
}
