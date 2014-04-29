
#pragma once

#include "declspec.h"

#include "abstracttimer.h"


class ChronoTimer;

// Timer is a wrapper of chronotimer to hide its stl usage from dll 
// export using the pimpl idiom. Its interface is modeled on a 
// stopwatch. The time updates have to be requested explicitly, thus
// simplifying usage between multiple recipients.

class CGSEE_API Timer : public AbstractTimer
{
public:
    Timer(
        const bool start = true,
        const bool auto_update = true);

    virtual ~Timer();

    virtual void setAutoUpdating(const bool auto_update);
    virtual const bool autoUpdating() const;

    virtual void update() const;
    virtual const long double elapsed() const;

    virtual const bool paused() const;

    virtual void start();
    virtual void pause();
    virtual void stop ();
    virtual void reset();

protected:
    ChronoTimer * m_chrono;
};
