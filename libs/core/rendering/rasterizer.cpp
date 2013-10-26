#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "rasterizer.h"
#include <core/framebufferobject.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

#include <core/gpuquery.h>

Rasterizer::Rasterizer(Camera & camera)
:   RenderTechnique(camera)
{
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::renderScene(const Program & program, FrameBufferObject * target)
{
    m_camera.drawWithPostprocessing(target);
}

