#pragma once

template <class DataBlockSubclass>
typename DataBlockSubclass::t_StandardPointer
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
typename DataBlockSubclass::t_StandardPointer
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