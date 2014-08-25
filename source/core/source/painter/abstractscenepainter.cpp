
#include <core/painter/abstractscenepainter.h>

#include <signalzeug/Signal.h>

#include <core/scenegraph/group.h>
#include <core/camera/abstractcamera.h>


AbstractScenePainter::AbstractScenePainter()
: AbstractPainter()
, m_scene(nullptr)
, m_camera(nullptr)
{
}

AbstractScenePainter::~AbstractScenePainter()
{
}

void AbstractScenePainter::assignCamera(AbstractCamera *camera)
{
    if(m_camera == camera)
        return;

    delete m_camera;
    m_camera = camera;

    m_camera->viewChangedSignal().connect<AbstractScenePainter>(this, &AbstractScenePainter::viewChanged);

    cameraChanged();
}

AbstractCamera * AbstractScenePainter::camera()
{
    return m_camera;
}

void AbstractScenePainter::assignScene(Group * scene)
{
    if(m_scene == scene)
        return;

    delete m_scene;
    m_scene = scene;

    sceneChanged();
}

Group * AbstractScenePainter::scene()
{
    return m_scene;
}

void AbstractScenePainter::sceneChanged()
{
    //TODO
}

void AbstractScenePainter::cameraChanged()
{
    onViewChanged();
}

void AbstractScenePainter::viewChanged()
{
    onViewChanged();
}
