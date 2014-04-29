
#include "abstractscenepainter.h"
#include <core/scenegraph/group.h>
#include <core/camera.h>

AbstractScenePainter::AbstractScenePainter()
: AbstractPainter()
, m_scene(nullptr)
{
}

AbstractScenePainter::~AbstractScenePainter()
{
    delete m_scene;
}

void AbstractScenePainter::assignScene(Group * scene)
{
    this->sceneChanged(scene);
    if (m_scene)
        delete retainScene();

    m_scene = scene;
    if (m_initialized)
        this->camera()->append(m_scene);
}

Group * AbstractScenePainter::retainScene()
{
    this->camera()->remove(m_scene, false);
    Group * scene = m_scene;
    m_scene = nullptr;
    return scene;
}

Group & AbstractScenePainter::getScene() const
{
    return *m_scene;
}

void AbstractScenePainter::sceneChanged(Group * scene)
{
}


