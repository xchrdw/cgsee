
#include "canvasexportconfig.h"

const CanvasExportConfig::t_namedUnits CanvasExportConfig::m_namedUnits = 
    CanvasExportConfig::t_namedUnits() 
    << "Pixels" 
    << "Inches" 
    << "Millimeters" 
    << "Centimeters"
    << "Points" 
    << "Picas";

const CanvasExportConfig::t_inchBasedMultiplicators CanvasExportConfig::m_multiplicators = 
    CanvasExportConfig::t_inchBasedMultiplicators() 
    << 1.0 /  1.00 // pixel are evaluated in context with a given ppi
    << 1.0 /  1.00
    << 1.0 / 25.40
    << 1.0 /  2.54
    << 1.0 / 72.00
    << 1.0 /  6.00;

CanvasExportConfig::CanvasExportConfig()
:   m_unit(Pixels)
,   m_sizei(QSizeF(4.4444, 3.3333)) // 320 x 240
,   m_ppi(72.0)
,   m_alpha(false)
{
}

namespace
{
    const int MIN_WIDTH  =     1; 
    const int MIN_HEIGHT =     1;

    const int MAX_WIDTH  = 16384; // 16 * 1024;
    const int MAX_HEIGHT = 16384; // 16 * 1024;
}

const QString CanvasExportConfig::ppiUnitName()
{
    return QString("Pixels/Inch");
}

const QStringList & CanvasExportConfig::unitNames()
{
    return m_namedUnits;
}

const CanvasExportConfig::e_Unit CanvasExportConfig::unit() const
{
    return m_unit;
}

void CanvasExportConfig::setUnit(const e_Unit unit)
{
    m_unit = unit;
}

void CanvasExportConfig::setWidth(const qreal width)
{
    qreal inches = width * m_multiplicators[m_unit];
    if(Pixels == m_unit)
        inches /= m_ppi;

    m_sizei.setWidth(inches);
}

void CanvasExportConfig::setHeight(const qreal height)
{
    qreal inches = height * m_multiplicators[m_unit];
    if(Pixels == m_unit)
        inches /= m_ppi;

    m_sizei.setHeight(inches);
}

const qreal CanvasExportConfig::width() const
{
    qreal width = m_sizei.width() / m_multiplicators[m_unit];
    if(Pixels == m_unit)
        return qRound(width * m_ppi);


    return width;
}

const qreal CanvasExportConfig::height() const
{
    qreal height = m_sizei.height() / m_multiplicators[m_unit];
    if(Pixels == m_unit)
        return qRound(height * m_ppi);

    return height;
}

const qreal CanvasExportConfig::ppi() const
{
    return m_ppi;
}

void CanvasExportConfig::setPixelPerInch(const qreal ppi)
{
    if(ppi == m_ppi)
        return;

    if(Pixels != m_unit)
    {
        m_ppi = ppi;
        return;
    }

    // recalculate size

    const qreal w = width();
    const qreal h = height();

    m_ppi = ppi;

    setWidth(w);
    setHeight(h);
}

const QSize CanvasExportConfig::size() const
{
    QSizeF pixelsf = m_sizei;
    pixelsf *= m_ppi;

    QSize pixels = pixelsf.toSize();
    
    pixels.setWidth(qBound(MIN_WIDTH, pixels.width(), MAX_WIDTH));
    pixels.setHeight(qBound(MIN_HEIGHT, pixels.height(), MAX_HEIGHT));

    return pixels;
}

const QString CanvasExportConfig::sizeString() const
{
    // size in bytes
    const QSize pixels = size();
    const int size = pixels.width() * pixels.height() * (m_alpha ? 4 : 3);

    qreal sizef = static_cast<qreal>(size);

    // shorten the size to nearest size unit with max 2 decimals

    int i = 0; 
    while(sizef >= 100.0)
    {
        sizef /= 1024.f;
        ++i;
    }

    static const QStringList sizeUnits = QStringList()
        << "B" << "KiB" << "MiB" << "GiB" ; // hope for not reaching TiB and up spaces... :P

    return QString("%1 %2 (%3 x %4 Pixels)")
        .arg(QString::number(sizef, 'g', 3))
        .arg(sizeUnits[i])
        .arg(pixels.width())
        .arg(pixels.height());
}