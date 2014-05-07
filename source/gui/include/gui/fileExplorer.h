#pragma once

#include <gui/gui_api.h>

#include <QListView>
#include <QDir>



class QFileSystemModel;
class QMenu;


class GUI_API FileExplorer : public QListView
{
	Q_OBJECT
	
public:
	FileExplorer(QWidget * parent = nullptr);
	virtual ~FileExplorer();

	QMenu * menu();
	QFileSystemModel * model();
	QModelIndex clickedFile();

	void setFilter(const QDir::Filters & filters);
	void setAllLoadableTypes(const QStringList & types);

signals:
	void activatedDir(const QString & path);
	void activatedItem(const QString & path);

public slots:
	void emitActivatedItem(const QModelIndex & index);
	void setClickedFile(const QModelIndex & index);
	void triggeredOpen(const bool & triggered);
	void showContextMenu(const QPoint & point);
	void setRoot(QString rootPath);

protected:
	QFileSystemModel * m_model;
	QMenu * m_menu;

	QModelIndex m_clickedFile;
};