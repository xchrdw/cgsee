
#pragma once

#include <chrono>

#include "abstracttimer.h"


class ChronoTimer : public AbstractTimer
{
public:
    ChronoTimer(
        const bool start = true,   
        const bool auto_update = true);

    virtual ~ChronoTimer();

    virtual void setAutoUpdating(const bool auto_update);
    virtual const bool autoUpdating() const;

    virtual const bool paused() const;

    virtual void update() const;
    virtual const long double elapsed() const;

    virtual void start();
    virtual void pause();
    virtual void stop ();
    virtual void reset();

protected:
    bool m_paused;
    bool m_auto;

protected:
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point time_point; 

    typedef std::chrono::duration<long double, std::nano> nano;

    time_point m_t0;
    time_point m_tp; // time_point of last pausing

    mutable time_point m_t1;

    long double m_offset;
    mutable long double m_elapsed;
};
