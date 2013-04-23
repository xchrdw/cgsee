#include "abstractdata.h"

QString GlobalDataRegistry::registerNewData(QString proposedName, DataItem* data)
{
	m_dataMap.insert(proposedName, data);
	this->connect(data, SIGNAL(destroyed(QObject*)), SLOT(dataItemRemoved(QObject*)));
	return proposedName;
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