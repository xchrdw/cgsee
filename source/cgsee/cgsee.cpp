#include "cgsee.h"

#include <core/datacore/datablock.h>
#include <gui/viewer.h>
#include <core/camera.h>
#include <core/navigation/arcballnavigation.h>
#include <core/coordinateprovider.h>
#include <core/glformat.h>

CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_registry(nullptr)
,   m_viewer(nullptr)
{
    
    m_registry = std::make_shared<DataBlockRegistry>();
    
    m_viewer = new Viewer(m_registry);
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    CoordinateProvider * coordinateProvider = new CoordinateProvider();
    m_viewer->setCoordinateProvider(coordinateProvider);

    m_viewer->show();

    m_viewer->on_loadFile("data/shadow_test.obj");
}

CGSee::~CGSee()
{
    delete m_viewer;
}
