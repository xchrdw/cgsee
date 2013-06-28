#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpainterproperty.h>

class AbstractPropertyVisitor;

class CGSEE_API IntProperty : public AbstractPainterProperty
{
public:
    IntProperty(QString name, QString description, int value = 0);
    virtual ~IntProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    int value() const;
    void setValue(int value);

protected:
    int m_value;
};
