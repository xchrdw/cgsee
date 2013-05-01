
#pragma once

#include <core/abstractapplication.h>

class Viewer;
class Painter;

class CGSee : public AbstractApplication
{
    Q_OBJECT
public:
    CGSee(int & argc, char ** argv);
    virtual ~CGSee();

public slots:
    void exportCanvasAsImage();

protected:
    Viewer * m_viewer;
    Painter * m_painter;
};
