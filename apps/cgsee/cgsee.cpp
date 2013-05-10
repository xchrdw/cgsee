#include "cgsee.h"
#include "painter.h"

#include <gui/canvas.h>
#include <gui/viewer.h>
#include <gui/imageexport.h>


CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    // Create Viewer

    m_viewer = new Viewer();
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    m_painter = new Painter();
    m_viewer->setPainter(m_painter);

    // Start

    m_viewer->show();

    // connect signals
    connect(m_viewer, SIGNAL(takeScreenshot()), this, SLOT(exportCanvasAsImage()));
}

CGSee::~CGSee()
{
    // NOTE: painter must be destroyed before viewer, since viewer deletes context, 
    // and painter deinitializes all opengl handles...

    delete m_painter;
    delete m_viewer;
}

void CGSee::exportCanvasAsImage()
{
    ImageExport::save(*m_painter, m_viewer);
}
