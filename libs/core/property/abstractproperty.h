
#pragma once

#include <typeinfo>
#include <QObject>
#include <QString>
#include <core/declspec.h>

class AbstractPropertyVisitor;

class CGSEE_API AbstractProperty : public QObject
{
    Q_OBJECT

public:
    AbstractProperty(QString name, QString description);
    virtual ~AbstractProperty();

    virtual void visit(AbstractPropertyVisitor & visitor) = 0;

    QString name() const;
    void setName(QString name);
    QString description() const;
    void setDescription(QString name);

    template <class PropertyClass>
    PropertyClass * to();

signals:
    void changed(AbstractProperty & me);

protected:
    QString m_name;
    QString m_description;
};

template <class PropertyClass>
PropertyClass * AbstractProperty::to()
{
    PropertyClass * property = dynamic_cast<PropertyClass *>(this);
    if (!property)
        qFatal("Requested Property \"%s\" is not of Type \"%s\"", 
            qPrintable(this->name()), typeid(PropertyClass).name());
    return property;
}
