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

public slots:
	// Notifies about destruction of a DataItem.
	void dataItemRemoved(QObject* dataItem);

protected:
	QMap<QString, DataItem*> m_dataMap;
};

class CGSEE_API DataItem: public QObject
{
	Q_OBJECT
public:
	DataItem();
	virtual ~DataItem();

	QString name() const;
	Q_PROPERTY(QString name READ name)
protected:
	QString m_name;

};

#endif //DATAINTERFACE_H