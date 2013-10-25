#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "rasterizer.h"
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

void Rasterizer::renderScene(const Program & program, const glm::mat4 & transform)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glError();

    SceneTraverser traverser;
    DrawVisitor drawVisitor(&program, m_camera.transform());
    traverser.traverse(m_camera, drawVisitor);
}
