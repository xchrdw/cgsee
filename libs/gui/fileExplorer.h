
#pragma once



#include <QListView>
#include <QDir>

#include <core/declspec.h>


class QFileSystemModel;
class QMenu;


class CGSEE_API FileExplorer : public QListView
{
	Q_OBJECT
	
public:
	FileExplorer(QObject * parent = nullptr);
	virtual ~FileExplorer();

	QFileSystemModel * model();
	QMenu * menu();
	QModelIndex clickedFile();

	void setFilter(const QDir::Filters & filters);

signals:
	void activatedItem(const QString & path);

public slots:
	// void callSetRoot(const QModelIndex & index);
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