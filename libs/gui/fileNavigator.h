
#pragma once



#include <QTreeView>
#include <QFileSystemModel>

#include <core/declspec.h>


class FileExplorer;


class CGSEE_API FileNavigator : public QTreeView
{
	Q_OBJECT

public:
	FileNavigator(QObject * parent = nullptr);
	virtual ~FileNavigator();

	FileExplorer * explorer();

	void setFilter(const QDir::Filters & filters);
	void setExplorer(FileExplorer * fileExplorer);

protected:
	void setRoot(const QString & rootPath);

	FileExplorer * m_explorer;
	QFileSystemModel * m_model;
};