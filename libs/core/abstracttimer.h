
#pragma once

#include "declspec.h"


// This interface is modeled as a stopwatch. The time updates can be 
// requested explicitly, thus simplifying usage between multiple recipients.

class CGSEE_API AbstractTimer
{
public:
    virtual void setAutoUpdating(const bool auto_update) = 0;
    virtual const bool autoUpdating() const = 0;

    virtual void update() const = 0;
    virtual const long double elapsed() const = 0;

    virtual const bool paused() const = 0;

    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void stop () = 0;
    virtual void reset() = 0;

protected:
    AbstractTimer() {}
};
