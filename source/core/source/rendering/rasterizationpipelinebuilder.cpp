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

bool addRenderingStages(MonoCameraNew * camera)
{

    return true;
}

bool addPostProcessingStages(MonoCameraNew * camera)
{

    return true;
}

