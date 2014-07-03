#include <core/rendering/rasterizationpipelinebuilder.h>
#include <core/camera/monocamera.h>
#include <core/camera/stereocamera.h>

RasterizationPipelineBuilder::RasterizationPipelineBuilder(PipelinePainter & painter)
    : PipelineBuilder(painter)
{

}

RasterizationPipelineBuilder::~RasterizationPipelineBuilder()
{

}

bool addRenderingStages(const MonoCameraNew * camera)
{
    //TODO
    return true;
}

bool addPostProcessingStages(const MonoCameraNew * camera)
{
    //TODO
    return true;
}

