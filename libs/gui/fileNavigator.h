
#pragma once



#include <QTreeView>
#include <QFileSystemModel>
#include <QDockWidget>

#include <core/declspec.h>


class FileExplorer;


class CGSEE_API FileNavigator : public QTreeView
{
	Q_OBJECT

public:
	FileNavigator(QObject * parent = nullptr);
	virtual ~FileNavigator();

	void setFilter(QDir::Filters filters);

	void setExplorer(FileExplorer * fileExplorer);
	FileExplorer * explorer();
	QDockWidget * dock();

protected:
	void setRoot(QString rootPath);

	FileExplorer * m_explorer;
	QFileSystemModel * m_model;
	QDockWidget * m_dock;
};