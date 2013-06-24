
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

    virtual void visit(AbstractPropertyVisitor & visitor) const =0;
    
    QString name() const;
    void setName(QString name);
private:
    QString m_name;
};