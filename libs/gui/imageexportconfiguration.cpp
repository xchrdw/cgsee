
#include "CanvasExportConfig.h"

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
    <<  0.00 // pixel are evaluated in context with a given ppi
    <<  1.00
    << 25.40
    <<  2.54
    << 72.00
    <<  6.00;

const QStringList CanvasExportConfig::unitNames()
{
    return m_namedUnits;
}