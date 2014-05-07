﻿#pragma once

#include <core/core_api.h>

#include <QtCore>

class DataBlockRegistry;

// Base class for all DataBlocks in DataCore. Provides some basic functionality.
// All data blocks are created using createDataBlockWithName<T>(..) functions, 
// and are automatically registered in provided Registry.
// 
// QString vListName("VertexList"); 
// t_VertexListP vList = DataBlock::createDataBlockWithName<VertexList>(vListName, registry);
// // vListName contains now true name of the data block.
// 
// The reasons to use separate function instead of class constructors are:
// - the data blocks are guaranteed to be registered in the Registry automatically
// - the data blocks can be wrapped in smart pointers, and they will be eventually
// - If something exceptional happens, we can simply return nullptr.

class CORE_API DataBlock: public QObject
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

    // Alternatively, you can use DataBlock::deleteLater() member func. 
    template<class DataBlockSubclass>
    static void destroyDataBlock(typename DataBlockSubclass::t_StandardPointer obj);

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

class CORE_API DataBlockRegistry: public QObject
{
    Q_OBJECT
public:
    DataBlockRegistry(const QString &objName = QString(), QObject * parent = nullptr); 
    virtual ~DataBlockRegistry();

    // This Method registers the new data item using the name given; 
    // if an item with this name already exists, GlobalDataRegistry will
    // choose an unique name on its own; 
    // returns the name under which the object has been saved
    QString registerNewData(QString proposedName, t_DataBlockP data);
    t_DataBlockP getDataBlockByName(QString itemName) const;

signals:
    void dataBlockAdded(QObject* dataBlock);

public slots:
    // Notifies about destruction of a DataItem.
    void dataBlockRemoved(QObject* dataBlock);

protected:
    // TODO: Multihash -> für bessere hierarchie-unterstützung?
    QHash<QString, t_DataBlockP> m_dataMap;
};

#include "datablock_impl.tli"
