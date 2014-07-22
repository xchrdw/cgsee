#include <core/rendering/ssaostage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

SsaoStage::SsaoStage(PipelinePainter & painter)
    : AbstractPostProcessingStage(painter)
{
    //m_program->attach(myFancyShader);
}

SsaoStage::~SsaoStage(void)
{
}

void SsaoStage::reloadShaders()
{
    //TODO
}

void SsaoStage::resize(const int width, const int height)
{
    //TODO
}
