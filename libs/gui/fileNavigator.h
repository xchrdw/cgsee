
#pragma once



#include <QTreeView>
#include <QDir>

#include <core/declspec.h>


class QFileSystemModel;


class CGSEE_API FileNavigator : public QTreeView
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

	QFileSystemModel * m_model;
};