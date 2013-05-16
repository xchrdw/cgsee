
#pragma once



#include <QListView>
#include <QFileSystemModel>
#include <QMenu>

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

	void loadFile(const QModelIndex & index);

public slots:
	void callSetRoot(const QModelIndex & index);
	void showContextMenu(const QPoint & point);
	void triggeredLoadFile(const bool & triggered);

protected:
	void setRoot(QString rootPath);

	FileNavigator * m_navigator;
	QFileSystemModel * m_model;
	QMenu * m_menu;
	
	QModelIndex m_clickedFile;
};