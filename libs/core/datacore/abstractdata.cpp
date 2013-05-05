#include "abstractdata.h"

DataBlock::DataBlock() // protected
{
}

DataBlock::DataBlock(const DataBlock &) // protected
{
}

DataBlock::~DataBlock() // virtual
{
}

QString DataBlock::name() const
{
    return m_name;
}

t_DataBlockP DataBlock::clone(QString *newName, DataBlockRegistry & registry)
{
    t_DataBlockP newItem = new DataBlock(*this);
    QString chosenName = (nullptr == newName) ? name() : *newName;
    chosenName = registry.registerNewData(chosenName, newItem);
    if (newName)
        *newName = chosenName;
    return newItem;
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
    this->connect(data, SIGNAL(destroyed(QObject*)), SLOT(dataItemRemoved(QObject*)));
    return realName;
}

t_DataBlockP DataBlockRegistry::getDataItemByName(QString name)
{
    return m_dataMap[name];
}

void DataBlockRegistry::dataItemRemoved(QObject* removedItem)
{
    auto iter = qFind(m_dataMap.begin(), m_dataMap.end(), removedItem);
    if (iter != m_dataMap.end())
        m_dataMap.erase(iter);
}
