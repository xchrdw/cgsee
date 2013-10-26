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
{/*
    if (target != nullptr)
        target->bind();*/

    /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glError();*/
    m_camera.drawWithPostprocessing(target);

    //if (target != nullptr)
    //    target->release();
}
