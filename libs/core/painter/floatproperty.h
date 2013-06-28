#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpainterproperty.h>

class AbstractPropertyVisitor;

class CGSEE_API FloatProperty : public AbstractPainterProperty
{
public:
    FloatProperty(QString name, QString description, double value = 0);
    virtual ~FloatProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    double value() const;
    void setValue(double value);

protected:
    double m_value;
};
