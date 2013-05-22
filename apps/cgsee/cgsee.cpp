#include "cgsee.h"
#include "painter.h"
#include "deferredpainter.h"

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

    //m_painter = new Painter();
    m_painter = new DeferredPainter();
    m_viewer->setPainter(m_painter);

    // Start

    m_viewer->show();
}

CGSee::~CGSee()
{
    // NOTE: painter must be destroyed before viewer, since viewer deletes context, 
    // and painter deinitializes all opengl handles...

    delete m_painter;
    delete m_viewer;
}
