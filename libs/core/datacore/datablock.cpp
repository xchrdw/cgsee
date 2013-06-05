#include "datablock.h"

DataBlock::DataBlock(QObject * parent):
    QObject(parent)
{
}

DataBlock::DataBlock(const DataBlock &) // protected
{
}

DataBlock::~DataBlock() // virtual
{
}

void DataBlock::triggerUpdatedSignal()
{
    emit updated(this);
}

t_DataBlockP DataBlock::createClone()
{
    return new DataBlock(*this);
}

DataBlockRegistry::DataBlockRegistry(QString const &objName, QObject *parent):
    QObject(parent)
{
    setObjectName(objName);
}

DataBlockRegistry::~DataBlockRegistry() // virtual
{
}

QString DataBlockRegistry::registerNewData(QString proposedName, t_DataBlockP data)
{
    static const QString extraFormat("%1%2");
    QString realName = proposedName;
    if (m_dataMap.contains(proposedName))
    {
        unsigned int i = 0;
        do {
            realName = extraFormat.arg(proposedName).arg(++i);
        } while(m_dataMap.contains(realName));
    }
    m_dataMap.insert(realName, data);
    this->connect(data, SIGNAL(destroyed(QObject*)), SLOT(dataBlockRemoved(QObject*)));

    emit dataBlockAdded(data);
    return realName;
}

t_DataBlockP DataBlockRegistry::getDataBlockByName(QString name) const
{
    return m_dataMap[name];
}

void DataBlockRegistry::dataBlockRemoved(QObject* removedItem)
{
    auto iter = qFind(m_dataMap.begin(), m_dataMap.end(), removedItem);
    if (iter != m_dataMap.end())
        m_dataMap.erase(iter);
}
