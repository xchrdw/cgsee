#pragma once

#include <gui/gui_api.h>

#include <QTreeView>
#include <QDir>


class QFileSystemModel;


class GUI_API FileNavigator : public QTreeView
{
	Q_OBJECT

public:
	FileNavigator(QWidget * parent = nullptr);
	virtual ~FileNavigator();


	void setFilter(const QDir::Filters & filters);

signals:
	void clickedDirectory(const QString & path);

public slots:
	void emitClickedDirectory(const QModelIndex & index);
    void on_activatedDir(const QString & path);

protected:
	void setRoot(const QString & rootPath);

	QFileSystemModel * m_model;
};