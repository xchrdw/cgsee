#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H
#pragma once

#include <QtCore>

#include "../declspec.h"

class DataItem;

class CGSEE_API GlobalDataRegistry: public QObject
{
    Q_OBJECT
public:
    virtual ~GlobalDataRegistry(){}

    // This Method registers the new data item using the name given; 
    // if an item with this name already exists, GlobalDataRegistry will
    // choose an unique name on its own; 
    // returns the name under which the object has been saved
    QString registerNewData(QString proposedName, DataItem* data);
    DataItem* getDataItemByName(QString itemName);
    static GlobalDataRegistry& getGlobalInstance()
    {
        return s_instance;
    }

public slots:
    // Notifies about destruction of a DataItem.
    void dataItemRemoved(QObject* dataItem);

protected:
    QHash<QString, DataItem*> m_dataMap;

private:
    static GlobalDataRegistry s_instance;
};

class CGSEE_API DataItem: public QObject
{
    Q_OBJECT
public:
    virtual ~DataItem(){}

    typedef DataItem * StandardPointer;

    template<class DataItemSubclass>
    static StandardPointer createDataItemWithName(QString &name, GlobalDataRegistry &registry)
    {
        DataItem result;
        result.m_name = registry.registerNewData(name, &result);
    }

    // attempt to clone the item.
    // it will be given a new name and saved in the registry provided.
    virtual StandardPointer clone(QString *newName, GlobalDataRegistry & registry);
    virtual StandardPointer clone(QString *newName);

    QString name() const;
    Q_PROPERTY(QString name READ name)
protected:
    QString m_name;
    
    // Objects are created with static function createDataItemWithName which
    // registers them automatically in the registry provided
    DataItem(){}

    // This is already done in QObject, but in case if anyone forgets:
    // QObjects cannot be copied with a constructor
    DataItem(const DataItem&){}
};

typedef DataItem::StandardPointer DataItemP;

#endif //DATAINTERFACE_H