
#pragma once

#include <QString>
#include <core/declspec.h>

class AbstractPropertyVisitor;

class CGSEE_API AbstractPainterProperty
{
public:
    AbstractPainterProperty();
    explicit AbstractPainterProperty(QString name);
    AbstractPainterProperty(QString name, QString description);
    virtual ~AbstractPainterProperty();

    virtual void visit(AbstractPropertyVisitor & visitor) =0;
    
    QString name() const;
    void setName(QString name);
    QString description() const;
    void setDescription(QString name);
protected:
    QString m_name;
    QString m_description;
};