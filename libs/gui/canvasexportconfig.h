
#pragma once

#include <QVector>
#include <QStringList>
#include <QSize>
#include <QSizeF>
#include <QMap>

class CanvasExportConfig
{
public:
    enum e_Unit
    {
        Pixels
    ,   Inches
    ,   Millimeters
    ,   Centimeters
    ,   Points
    };

protected:
    static QStringList m_presetIDs;

    typedef QStringList t_namedUnits;
    static const t_namedUnits m_namedUnits;

    typedef QVector<qreal> t_inchBasedMultiplicators;
    static const t_inchBasedMultiplicators m_multiplicators;;

public:
    CanvasExportConfig(
        const qreal width  = 4.4444
    ,   const qreal height = 3.3333
    ,   const qreal ppi    = 72.0
    ,   const e_Unit unit  = Inches);

    const QSize size() const;

    static const QStringList & unitNames();
    static const QString ppiUnitIdentifier();

    static const QStringList & presetsIdentifier();
    static CanvasExportConfig * preset(const QString & identifier);

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

    const bool modified() const;

protected:
    e_Unit  m_unit;

    QSizeF  m_sizei; // size in inches
    qreal   m_ppi;  // pixel per inch

    bool    m_alpha;
    bool    m_aspect;

    bool    m_modified;
};