
#pragma once



#include <QListView>
#include <QFileSystemModel>

#include <core/declspec.h>


class FileNavigator;


class CGSEE_API FileExplorer : public QListView
{
	Q_OBJECT
	
public:
	FileExplorer(QObject * parent = nullptr);
	virtual ~FileExplorer();

	void setFilter(QDir::Filters filters);

	void setNavigator(FileNavigator * fileNavigator);
	FileNavigator * navigator();

public slots:
	void callSetRoot(const QModelIndex & index);

protected:
	void setRoot(QString rootPath);

	FileNavigator * m_navigator;
	QFileSystemModel * m_model;
};