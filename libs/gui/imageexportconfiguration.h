
#pragma once

#include <QVector>
#include <QStringList>
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
    static const QStringList unitNames();

protected:

    QSizeF m_sizei; // size in inch
    qreal  m_ppi;   // pixel per inch
};