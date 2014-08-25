#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <core/property/valueproperty.h>
#include <core/painter/pipelinepainter.h>
#include <core/rendering/abstractrenderstage.h>


class CORE_API AASetupStage : public AbstractRenderStage
{
public:
    AASetupStage(PipelinePainter & painter);
    virtual ~AASetupStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
    virtual void render() override;

protected:
    static glm::vec2 randomOffset(double kernelSize);

protected:
    glm::vec2 m_pixelSize;
};
