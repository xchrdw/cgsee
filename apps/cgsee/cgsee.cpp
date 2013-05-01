
#include "cgsee.h"
#include "painter.h"

#include <gui/viewer.h>

#include <QImageReader>
#include <QFileDialog>

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
    connect(m_viewer, SIGNAL(takeScreenshot()), this, SLOT(takeScreenshot()));
}

CGSee::~CGSee()
{
    delete m_viewer;
    delete m_painter;
}

void CGSee::takeScreenshot()
{
    // get supported image formats
    QString ext = tr("All files (*.*)");
    QStringList fmts;
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    for (QList<QByteArray>::iterator i = formats.begin();
        i != formats.end(); ++i) {
            fmts.push_back(QString("*.%0").arg(QString::fromLocal8Bit(*i).toLower()));
    }
    ext = tr("Image files (%1)\nAll files (*.*)").arg(fmts.join(" "));

    QString fileName = QFileDialog::getSaveFileName(m_viewer, QString("Save screenshot as..."), QString(), ext);
    if (fileName.isEmpty())
        return;

    const QImage frame(m_painter->capture());
    if(!frame.isNull())
        frame.save(fileName);
}
