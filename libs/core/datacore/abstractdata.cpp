#include "abstractdata.h"

QString GlobalDataRegistry::registerNewData(QString proposedName, DataItem* data)
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

DataItem * GlobalDataRegistry::getDataItemByName(QString name)
{
    return m_dataMap[name];
}

void GlobalDataRegistry::dataItemRemoved(QObject* removedItem)
{
    auto iter = qFind(m_dataMap.begin(), m_dataMap.end(), removedItem);
    if (iter != m_dataMap.end())
        m_dataMap.erase(iter);
}

QString DataItem::name() const
{
    return m_name;
}

DataItem::StandardPointer DataItem::clone(QString *newName, GlobalDataRegistry & registry)
{
    DataItem::StandardPointer newItem = new DataItem(*this);
    QString chosenName = (nullptr == newName) ? name() : *newName;
    chosenName = registry.registerNewData(chosenName, newItem);
    if (newName)
        *newName = chosenName;
    return newItem;
}

DataItem::StandardPointer DataItem::clone(QString *newName)
{
    return clone(newName, GlobalDataRegistry::getGlobalInstance());
}

GlobalDataRegistry GlobalDataRegistry::s_instance;