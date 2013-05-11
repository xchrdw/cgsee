
#pragma once

#include <QVector>
#include <QStringList>
#include <QSize>
#include <QSizeF>

class CanvasExportConfig
{
public:
    enum e_Unit
    {
        Pixels
    ,   Inches
    ,   Centimeters
    ,   Millimeters
    ,   Points
    ,   Pica
    };

protected:
    typedef QStringList t_namedUnits;
    static const t_namedUnits m_namedUnits;

    typedef QVector<qreal> t_inchBasedMultiplicators;
    static const t_inchBasedMultiplicators m_multiplicators;

public:
    CanvasExportConfig();

    const QSize size() const;

    static const QStringList & unitNames();
    static const QString ppiUnitName();

    const e_Unit unit() const;
    void setUnit(const e_Unit unit);

    const qreal ppi() const;
    void setPixelPerInch(const qreal ppi);

    const qreal width() const;
    void setWidth(const qreal width);

    const qreal height() const;
    void setHeight(const qreal height);

    const QString sizeString() const;

    const bool aspect() const;
    void setAspect(const bool enforce);

    const bool alpha() const;
    void setAlpha(const bool use);

protected:
    e_Unit  m_unit;

    QSizeF  m_sizei; // size in inches
    qreal   m_ppi;  // pixel per inch

    bool    m_alpha;
    bool    m_aspect;
};