
#pragma once

#include <core/abstractapplication.h>

class Viewer;
class Painter;

class CGSee : public AbstractApplication
{
public:
    CGSee(int & argc, char ** argv);
    virtual ~CGSee();

protected:
    Viewer * m_viewer;
    Painter * m_painter;
};
