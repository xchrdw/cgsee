
#pragma once

#include <QStringList>
#include <core/declspec.h>
#include "abstractpainterproperty.h"

class CGSEE_API ListProperty : public AbstractPainterProperty
{
public:
    ListProperty();
    explicit ListProperty(QString name);
    ListProperty(QString name, QString description);
    virtual ~ListProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    virtual ListProperty * toList();

    QStringList & list() const;
    QString selection() const;

    bool insert(QString string);
    bool insertList(QStringList strings);
    bool remove(QString string);
    bool select(QString string);

protected:
    static const int kNoIndex = -1;
    QStringList * m_list;
    int m_selection;
};