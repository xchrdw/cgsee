#include "cgsee.h"

#include <core/datacore/datablock.h>
#include <gui/viewer.h>
#include <core/camera/abstractcamera.h>
#include <core/camera/monocamera.h>
#include <core/navigation/arcballnavigation.h>
#include <core/coordinateprovider.h>
#include <core/painter/pipelinepainter.h>


CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_registry(nullptr)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    
    m_registry = std::make_shared<DataBlockRegistry>();
    
    m_viewer = new Viewer(m_registry);
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    AbstractCamera * camera = new MonoCamera("main");
    camera->selectImplementation("MonoCamera");
    
    camera->setFovy (45.0f);
    camera->setZNear( 1.0f);
    camera->setZFar (20.0f);
    // camera->setZFar (300.0f);
    m_viewer->setCamera(camera);

    m_painter = new PipelinePainter(camera);
    m_viewer->setPainter(m_painter);

    AbstractNavigation * navigation = new ArcballNavigation(camera);
    m_viewer->setNavigation(navigation);

    CoordinateProvider * coordinateProvider = new CoordinateProvider();
    m_viewer->setCoordinateProvider(coordinateProvider);

    m_viewer->show();

    m_viewer->on_loadFile("data/shadow_test.obj");
}

CGSee::~CGSee()
{
    // NOTE: painter must be destroyed before viewer, since viewer deletes context, 
    // and painter deinitializes all opengl handles...

    delete m_painter;
    delete m_viewer;
}
