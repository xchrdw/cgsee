
#include <cassert>
#include <cmath>

#include "autotimer.h"
#include "timer.h"


int AutoTimer::m_numActiveInstances(0);

const QByteArray AutoTimer::m_units("num ");


AutoTimer::AutoTimer(const QString & info)
:   m_index(++m_numActiveInstances)
,   m_timer(new Timer(false))
,   m_info(info)
{
    m_timer->start();
}

AutoTimer::~AutoTimer()
{
    m_timer->pause();

    double delta(static_cast<double>(m_timer->elapsed()));

    // use number of digits to retrieve exp in 10^(3 exp)
    const unsigned char u(qMin<unsigned char>(m_units.length() - 1
        , ceil(log10(delta) / 3.0)));

    // shorten the time to nearest time unit
    delta /= pow(1000.0, u);

    qDebug("%s took %.4f %cs (Timer%02i)", qPrintable(m_info)
        , delta, m_units[u], m_index);
    delete m_timer;

    assert(0 < m_numActiveInstances);
    --m_numActiveInstances;
}