
#pragma once

#include <QString>
#include <core/declspec.h>

class AbstractPropertyVisitor;

class CGSEE_API AbstractPainterProperty
{
public:
    AbstractPainterProperty();
    explicit AbstractPainterProperty(QString name);
    virtual ~AbstractPainterProperty();

    virtual void visit(AbstractPropertyVisitor & visitor) =0;
    
    QString name() const;
    void setName(QString name);
protected:
    QString m_name;
};