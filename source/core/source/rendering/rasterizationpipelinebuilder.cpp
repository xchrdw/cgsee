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

bool RasterizationPipelineBuilder::addRenderingStages(const MonoCameraNew * camera)
{
    
    return true;
}

bool RasterizationPipelineBuilder::addPostProcessingStages(const MonoCameraNew * camera)
{
    //TODO
    return true;
}

