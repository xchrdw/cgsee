#include <core/rendering/rasterizationpipelinebuilder.h>
#include <core/camera/monocamera.h>
#include <core/camera/stereocamera.h>

#include <core/rendering/aasetupstage.h>
#include <core/rendering/aaaccumulatestage.h>
#include <core/rendering/renderstage.h>

RasterizationPipelineBuilder::RasterizationPipelineBuilder(PipelinePainter & painter)
    : AbstractPipelineBuilder(painter)
{

}

RasterizationPipelineBuilder::~RasterizationPipelineBuilder()
{

}

bool RasterizationPipelineBuilder::buildPipeline(const MonoCamera * camera)
{
    m_painter.clearPipeline();
    m_painter.setValue<bool>("viewInvalid", true);

    m_painter.addRenderStage(new AASetupStage(m_painter));

    QString normalzBufferName = QString(camera->name()).append("_normalz");
    m_painter.addRenderStage(new RenderStage(m_painter, normalzBufferName));

    QString accumulateBufferName = QString(camera->name()).append("_accumulate");
    m_painter.addRenderStage(new AAAccumulateStage(m_painter, normalzBufferName, accumulateBufferName));

    m_painter.setValue<std::string>("selectedBufferName", accumulateBufferName.toStdString());

    return true;
}

