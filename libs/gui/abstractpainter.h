#pragma once

#include <QStringList>

#include <core/declspec.h>
#include <core/glformat.h>


class CGSEE_API AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    virtual void paint();
    virtual void resize(const int width, const int height);
    virtual bool objectDetected(const QPoint &position);
    virtual void doMagic();

protected:
    virtual const bool initialize() = 0;

protected:
    bool m_initialized;
};
