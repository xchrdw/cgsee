#include <core/rendering/aasetupstage.h>

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

glm::vec2 AASetupStage::s_offset[] = {
        glm::vec2(0.375, 0.375), glm::vec2(0.375, -0.375), glm::vec2(-0.375, -0.375), glm::vec2(-0.375, 0.375),
        glm::vec2(0.375, 0.125), glm::vec2(0.125, -0.375), glm::vec2(-0.375, -0.125), glm::vec2(-0.125, 0.375),
        glm::vec2(0.125, 0.125), glm::vec2(0.125, -0.125), glm::vec2(-0.125, -0.125), glm::vec2(-0.125, 0.125),
        glm::vec2(0.125, 0.375), glm::vec2(0.375, -0.125), glm::vec2(-0.125, -0.375), glm::vec2(-0.375, 0.125)
};

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
        if (accumulatedSubpixels < 16)
        {
            glm::vec2 offset = s_offset[accumulatedSubpixels];
            painter().setProjection(
                        glm::translate(painter().projection(),
                                       glm::vec3(offset.x * m_pixelSize.x * 5.0,
                                                 offset.y * m_pixelSize.y * 5.0, 0)));
        }
    }
}
