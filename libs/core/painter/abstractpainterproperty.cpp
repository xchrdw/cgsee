
#include "abstractpainterproperty.h"

QString AbstractPainterProperty::name() const
{
    return m_name;
}

void AbstractPainterProperty::setName(QString name)
{
    m_name = name;
}

AbstractPainterProperty::~AbstractPainterProperty()
{
}