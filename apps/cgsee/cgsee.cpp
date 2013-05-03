
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
    delete m_viewer;
    delete m_painter;
}

void CGSee::exportCanvasAsImage()
{
    ImageExport ie(m_viewer);
    if(!ie.show())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    // TODO: Size GUI in ImageExport...

    ie.save(m_viewer->canvas()->capture(QSize(10000, 5000)));

    QApplication::restoreOverrideCursor();
}
