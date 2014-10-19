#include <core/rendering/aasetupstage.h>

#include <cstdlib>

#include <core/gpuquery.h>

#include <core/painter/pipelinepainter.h>

AASetupStage::AASetupStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
    , m_pixelSize(0.0, 0.0)
{
    painter.addProperty<int>("accumulatedSubpixels", 0);
}

AASetupStage::~AASetupStage()
{

}

void AASetupStage::reloadShaders()
{

}

void AASetupStage::resize(const int width, const int height)
{
    m_pixelSize = glm::vec2(2.0 / width, 2.0 / height);
}

void AASetupStage::render()
{
    if(painter().value<bool>("viewInvalid"))
    {
        painter().setValue<int>("accumulatedSubpixels", 0);
    }
    else
    {
        int accumulatedSubpixels = painter().value<int>("accumulatedSubpixels");
        if (accumulatedSubpixels < 64)
        {
            glm::vec2 offset = randomOffset(1.35);
            painter().setProjection(
                        glm::translate(glm::mat4(),
                                       glm::vec3(offset.x * m_pixelSize.x,
                                                 offset.y * m_pixelSize.y, 0)) * painter().projection());
        }
    }
}

glm::vec2 AASetupStage::randomOffset(double kernelSize)
{
    return glm::vec2(static_cast<double>(rand() % 1000) / 1000.0 * kernelSize - kernelSize / 2,
                     static_cast<double>(rand() % 1000) / 1000.0 * kernelSize - kernelSize / 2);
}
