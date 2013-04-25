
#include "cgsee.h"
#include "painter.h"

#include <gui/viewer.h>
#include <core/gpuquery.h>


CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    // Create Viewer

    glError();

    m_viewer = new Viewer();
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    m_painter = new Painter();
    m_viewer->setPainter(m_painter);

    // Start

    m_viewer->show();
}

CGSee::~CGSee()
{
    delete m_viewer;
    delete m_painter;
}
