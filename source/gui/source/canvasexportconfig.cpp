
#include "canvasexportconfig.h"

#include <cassert>  



const CanvasExportConfig::t_namedUnits CanvasExportConfig::m_namedUnits = 
    CanvasExportConfig::t_namedUnits() 
    << QObject::tr("Pixels")
    << QObject::tr("Inches")
    << QObject::tr("Millimeters")
    << QObject::tr("Centimeters")
    << QObject::tr("Points");

const CanvasExportConfig::t_inchBasedMultiplicators CanvasExportConfig::m_multiplicators = 
    CanvasExportConfig::t_inchBasedMultiplicators() 
    << 1.0 /  1.00 // pixel are evaluated in context with a given ppi
    << 1.0 /  1.00
    << 1.0 / 25.40
    << 1.0 /  2.54
    << 1.0 / 72.00
    << 1.0 /  6.00;


QStringList CanvasExportConfig::m_presetIDs;

const QStringList & CanvasExportConfig::presetsIdentifier()
{
    if(!m_presetIDs.isEmpty())
        return m_presetIDs;

    m_presetIDs << "- U.S. PAPER -----------";
    m_presetIDs << "Letter     8.5 x 11.0 in";
    m_presetIDs << "Legal      8.5 x 14.0 in";
    m_presetIDs << "Tabloid   11.0 x 17.0 in";

    m_presetIDs << "- INTERNATIONAL PAPER --";
    m_presetIDs << "A0         841 x 1189 mm";
    m_presetIDs << "A1         594 x  841 mm";
    m_presetIDs << "A2         420 x  594 mm";
    m_presetIDs << "A3         297 x  420 mm";
    m_presetIDs << "A4         210 x  297 mm";
    m_presetIDs << "A5         148 x  210 mm";
    m_presetIDs << "A6         105 x  148 mm";
    
    m_presetIDs << "- EXT. GRAPHICS --------";
    m_presetIDs << "XGA       1024 x  768 px";
    m_presetIDs << "WXGA      1280 x  720 px";
    m_presetIDs << "WXGA      1280 x  768 px";
    m_presetIDs << "WXGA      1280 x  800 px";
    m_presetIDs << "WXGA      1360 x  768 px";
    m_presetIDs << "WXGA      1366 x  768 px";
    m_presetIDs << "XGA+      1152 x  864 px";
    m_presetIDs << "WXGA+     1440 x  900 px";
    m_presetIDs << "SXGA      1280 x 1024 px";
    m_presetIDs << "SXGA+     1400 x 1050 px";
    m_presetIDs << "WSXGA+    1680 x 1050 px";
    m_presetIDs << "UXGA      1600 x 1200 px";
    m_presetIDs << "WUXGA     1920 x 1200 px";

    m_presetIDs << "- QUAD EXT. GRAPHICS ---";
    m_presetIDs << "QWXGA     2048 x 1152 px";
    m_presetIDs << "QXGA      2048 x 1536 px";
    m_presetIDs << "WQXGA     2560 x 1600 px";
    m_presetIDs << "QSXGA     2560 x 2048 px";
    m_presetIDs << "WQSXGA    3200 x 2048 px";
    m_presetIDs << "QUXGA     3200 x 2400 px";
    m_presetIDs << "WQUXGA    3840 x 2400 px";

    m_presetIDs << "- HYPER EXT. GRAPHICS --";
    m_presetIDs << "HXGA      4096 x 3072 px";
    m_presetIDs << "WHXGA     5120 x 3200 px";
    m_presetIDs << "HSXGA     5120 x 4096 px";
    m_presetIDs << "WHSXGA    6400 x 4096 px";
    m_presetIDs << "HUXGA     6400 x 4800 px";
    m_presetIDs << "WHUXGA    7680 x 4800 px";

    m_presetIDs << "- STANDARD-DEFINITION --";
    m_presetIDs << "PAL        720 x  576 px";
    m_presetIDs << "NTSC       720 x  480 px";

    m_presetIDs << "- HIGH-DEFINITION ------";
    m_presetIDs << "nHD        640 x  360 px";
    m_presetIDs << "qHD        960 x  540 px";
    m_presetIDs << "HD        1280 x  720 px";
    m_presetIDs << "HD+       1600 x  900 px";
    m_presetIDs << "FHD       1920 x 1080 px";
    m_presetIDs << "QHD       2560 x 1440 px";
    m_presetIDs << "QHD+      3200 x 1800 px";
    m_presetIDs << "QFHD(4K)  3840 x 2160 px";
    m_presetIDs << "UHD(8K)   7680 x 4320 px";

    return m_presetIDs;
}

CanvasExportConfig * CanvasExportConfig::preset(const QString & identifier)
{
    // generate preset from identifier string
    // therefore the following format is expected:
    // "Named Preset Moep ...   <WIDTH> x <HEIGHT> <UNIT>"
    // e.g. "UHD(8K)   7680 x 4320 px"

    QStringList i = identifier.toLower().trimmed().split(QRegExp("\\s+"));
    int n = i.size();

    if(n < 4)
    {
        qWarning("Could not parse preset due to unknown identifer format\"%s\".", qPrintable(identifier));
        return nullptr;
    }
   
    const QString unit  (i[n - 1]);
    const QString width (i[n - 4]);
    const QString height(i[n - 2]);

    static QMap<QString, e_Unit> units;
    if(units.isEmpty())
    {
        units["px"] = Pixels;
        units["in"] = Inches;
        units["cm"] = Centimeters;
        units["mm"] = Millimeters;
        units["pt"] = Points;
    }
    if(!units.contains(unit))
    {
        qWarning("Could not parse preset due to unknown unit \"%s\".", qPrintable(unit));
        return nullptr;
    }

    return new CanvasExportConfig(width.toDouble(), height.toDouble()
        , units[unit] != Pixels ? 300 : 72, units[unit]);
}

CanvasExportConfig::CanvasExportConfig(
    const qreal width
,   const qreal height
,   const qreal ppi
,   const e_Unit unit)
:   m_unit(unit)
,   m_ppi(ppi)
,   m_alpha(false)
,   m_aspect(true)
{
    setWidth(width);
    setHeight(height);

    m_modified = false;
}

namespace
{
    const int MIN_WIDTH  =     1; 
    const int MIN_HEIGHT =     1;

    const int MAX_WIDTH  = 16384; // 16 * 1024;
    const int MAX_HEIGHT = 16384; // 16 * 1024;
}

const QString CanvasExportConfig::ppiUnitIdentifier()
{
    return QObject::tr("Pixels/Inch");
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
    if(unit == m_unit)
        return;

    m_unit = unit;
    m_modified = true;
}

void CanvasExportConfig::setWidth(const qreal width)
{
    qreal inches = width * m_multiplicators[m_unit];
    if(Pixels == m_unit)
        inches /= m_ppi;

    if(m_sizei.width() == inches)
        return;

    m_sizei.setWidth(inches);
    m_modified = true;
}

void CanvasExportConfig::setHeight(const qreal height)
{
    qreal inches = height * m_multiplicators[m_unit];
    if(Pixels == m_unit)
        inches /= m_ppi;

    if(m_sizei.height() == inches)
        return;

    m_sizei.setHeight(inches);
    m_modified = true;
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

    m_modified = true;

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

    return QObject::tr("%1 %2 (%3 x %4 Pixels)")
        .arg(QString::number(sizef, 'f', 3))
        .arg(sizeUnits[i])
        .arg(pixels.width())
        .arg(pixels.height());
}

const bool CanvasExportConfig::aspect() const
{
    return m_aspect;
}

void CanvasExportConfig::setAspect(const bool enforce)
{
    m_aspect = enforce;
}

const bool CanvasExportConfig::alpha() const
{
    return m_alpha;
}

void CanvasExportConfig::setAlpha(const bool use)
{
    m_alpha = use;
}

const bool CanvasExportConfig::modified() const
{
    return m_modified;
}
