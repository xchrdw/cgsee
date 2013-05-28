
#pragma once



#include <QListView>
#include <QDir>

#include <core/declspec.h>


class QFileSystemModel;
class QMenu;

class FileNavigator;


class CGSEE_API FileExplorer : public QListView
{
	Q_OBJECT
	
public:
	FileExplorer(QObject * parent = nullptr);
	virtual ~FileExplorer();

	FileNavigator * navigator();
	QFileSystemModel * model();
	QMenu * menu();
	QModelIndex clickedFile();

	void setNavigator(FileNavigator * fileNavigator);
	void setFilter(const QDir::Filters & filters);


public slots:
	void callSetRoot(const QModelIndex & index);
	void setClickedFile(const QModelIndex & index);
	void showContextMenu(const QPoint & point);

protected:
	void setRoot(QString rootPath);

	FileNavigator * m_navigator;
	QFileSystemModel * m_model;
	QMenu * m_menu;

	QModelIndex m_clickedFile;
};