
#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpainterproperty.h>

class AbstractPropertyVisitor;

class CGSEE_API BoolProperty : public AbstractPainterProperty
{
public:
    BoolProperty(QString name, QString description, bool enabled = false);
    virtual ~BoolProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    bool enabled() const;
    void setEnabled(bool enabled);
protected:
    bool m_enabled;
};
