
#pragma once

#include <QApplication>

#include "declspec.h"


class GLFormat;

// Application initializes Application strings from CMake generated Meta.
// It furhter installs a Message Handler for extended logging capabilities.

class CGSEE_API AbstractApplication : public QApplication
{
public:
    AbstractApplication(
        int & argc,
        char ** argv);

    virtual ~AbstractApplication();

    virtual const QString title() const;
    virtual const GLFormat format() const;
};
