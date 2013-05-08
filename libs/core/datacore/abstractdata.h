#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H
#pragma once

#include <QtCore>

#include "../declspec.h"

class DataBlockRegistry;

class CGSEE_API DataBlock: public QObject
{
    Q_OBJECT
public:
    typedef DataBlock * t_StandardPointer;

public:
    virtual ~DataBlock();

    template<class DataBlockSubclass>
    static typename DataBlockSubclass::t_StandardPointer 
        createDataBlockWithName(QString &name, DataBlockRegistry &registry, QObject* parent = nullptr);

    // support createDataBlockWithName<...>("mainCamera", registry)
    template<class DataBlockSubclass>
    static typename DataBlockSubclass::t_StandardPointer 
        createDataBlockWithName(QString const &name, DataBlockRegistry &registry, QObject* parent = nullptr);

    // attempt to clone the item.
    // it will be given a new name and saved in the registry provided.
    template <class DataBlockSubclass>
    typename DataBlockSubclass::t_StandardPointer 
        clone(QString &newName, DataBlockRegistry &registry, QObject* parent = nullptr);

    Q_PROPERTY(QString name READ objectName)

    virtual void triggerUpdatedSignal();

signals:
    void updated(QObject*);

protected:
    // Objects are created with static function createDataItemWithName which
    // registers them automatically in the registry provided
    explicit DataBlock(QObject *parent = nullptr);

    // This is already done in QObject, but in case if anyone forgets:
    // QObjects cannot be copied with a constructor
    DataBlock(const DataBlock&);

    virtual t_StandardPointer createClone();
};

typedef DataBlock::t_StandardPointer t_DataBlockP;

template <class DataBlockSubclass>
static typename DataBlockSubclass::t_StandardPointer
    DataBlock::createDataBlockWithName(QString& name, DataBlockRegistry &registry, QObject* parent)
{
    typedef typename DataBlockSubclass::t_StandardPointer t_RetType;

    t_RetType result = new DataBlockSubclass(parent);
    if (nullptr == result)
        return result;
    name = registry.registerNewData(name, result);
    result->setObjectName(name);
    return result;
}

template <class DataBlockSubclass>
static typename DataBlockSubclass::t_StandardPointer
    DataBlock::createDataBlockWithName(QString const &name, DataBlockRegistry &registry, QObject* parent)
{
    typedef typename DataBlockSubclass::t_StandardPointer t_RetType;

    t_RetType result = new DataBlockSubclass(parent);
    if (nullptr == result)
        return result;
    result->setObjectName(registry.registerNewData(name, result));
    return result;
}

template <class DataBlockSubclass>
typename DataBlockSubclass::t_StandardPointer
    DataBlock::clone(QString &newName, DataBlockRegistry &registry, QObject* parent)
{
    typedef typename DataBlockSubclass::t_StandardPointer t_RetType;

    t_RetType newItem = createClone();
    QString chosenName = (newName.isNull() || newName.isEmpty()) ? objectName() : newName;
    newName = registry.registerNewData(chosenName, newItem);
    newItem->setObjectName(newName);
    return newItem;
}

class CGSEE_API DataBlockRegistry: public QObject
{
    Q_OBJECT
public:
    DataBlockRegistry(const QString &objName, QObject * parent = nullptr); 
    virtual ~DataBlockRegistry();

    // This Method registers the new data item using the name given; 
    // if an item with this name already exists, GlobalDataRegistry will
    // choose an unique name on its own; 
    // returns the name under which the object has been saved
    QString registerNewData(QString proposedName, t_DataBlockP data);
    t_DataBlockP getDataBlockByName(QString itemName);

signals:
    void dataBlockAdded(QObject *dataBlock);

public slots:
    // Notifies about destruction of a DataItem.
    void dataBlockRemoved(QObject* dataBlock);

protected:
    // TODO: Multihash -> für bessere hierarchie-unterstützung?
    QHash<QString, t_DataBlockP> m_dataMap;
};

#endif //DATAINTERFACE_H