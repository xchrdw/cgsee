#include "cgsee.h"
#include "painter.h"

#include <core/assimploader.h>
#include <gui/canvas.h>
#include <gui/viewer.h>


CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    // Create Viewer

    m_viewer = new Viewer();
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    AssimpLoader loader;
    
    m_painter = new Painter(loader.importFromFile("data/mario.dae"));
    m_viewer->setPainter(m_painter);

    // Start

    m_viewer->show();
    
    m_painter->assignScene(loader.importFromFile("data/suzanne.obj"));
}

CGSee::~CGSee()
{
    // NOTE: painter must be destroyed before viewer, since viewer deletes context, 
    // and painter deinitializes all opengl handles...

    delete m_painter;
    delete m_viewer;
}
