
#include "viewer.h"

#include <QOpenGLContext>


#include <core/glformat.h>

#include "canvas.h"
#include "ui_viewer.h"


Viewer::Viewer(
    QWidget  * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)

,   m_qtCanvas(nullptr)
{
    m_ui->setupUi(this);
};

const HGLRC Viewer::currentContextHandle()
{
#ifdef WIN32
    return wglGetCurrentContext();
#else
    qFatal("Get current context implementation missing (glx).");
    return NULL;
#endif
}

const HGLRC Viewer::createQtContext(const GLFormat & format)
{
    m_qtCanvas = new Canvas(format, this);
    setCentralWidget(m_qtCanvas);

    QGLContext * qContext(m_qtCanvas->context());

    if(!qContext)
        qFatal("Creating QtGL-Context failed.");

    qContext->makeCurrent();

    if(QGLContext::currentContext() != qContext)
        qFatal("Making QtGL-Context current failed.");
    
    const HGLRC qtContextHandle = currentContextHandle();

    if(NULL == qtContextHandle)
        qFatal("Acquiring QtGL-Context handle failed.");

    // canvas verifies the context itself.

    qContext->doneCurrent();

    return qtContextHandle;
}

void Viewer::initialize(const GLFormat & format)
{
    if(!QGLFormat::hasOpenGL())
        qFatal("OpenGL not supported.");

    createQtContext(format);
}

Viewer::~Viewer()
{
    delete m_qtCanvas;
}
