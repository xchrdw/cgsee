#pragma once

#include <core/core_api.h>

#include <QApplication>

class GLFormat;

// Application initializes Application strings from CMake generated Meta.
// It furhter installs a Message Handler for extended logging capabilities.

class CORE_API AbstractApplication : public QApplication
{
public:
    AbstractApplication(
        int & argc,
        char ** argv);

    virtual ~AbstractApplication();

    virtual const QString title() const;
    virtual const GLFormat format() const;
};
