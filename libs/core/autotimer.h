
#pragma once

#include "declspec.h"

#include <QStringList>

class Timer;

// AutoTimer can be used for quick performance measurements.
// Just create an instance that, when deleted due to ending 
// scope, prints out the measured time. Example:
//
// {
//     AutoTimer t1("Initialization"); // starts timer
//     DoStuff();
// } // stops timer and prints out the elapsed time.
//
// If more control over time measurement is required, condier 
// using Timer directly.


class CGSEE_API AutoTimer
{
public:
    AutoTimer(const QString & info = QString());
    virtual ~AutoTimer();

protected:
    static int m_numActiveInstances;
    const static QStringList m_units;

    const QString m_info;
    const int m_index;

    Timer * m_timer;
};
