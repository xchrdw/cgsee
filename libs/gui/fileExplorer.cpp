
#include <QFileSystemModel>
#include <QMenu>

#include "fileExplorer.h"



FileExplorer::FileExplorer(
	QObject * parent)

:	m_model(new QFileSystemModel)
,	m_menu(new QMenu)
,	m_clickedFile()
{
	setModel(m_model);
	setContextMenuPolicy(Qt::CustomContextMenu);

	setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setRoot("");

	m_model->setNameFilterDisables(false);

	QObject::connect(
		this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));
};

FileExplorer::~FileExplorer()
{
	delete m_model;
	delete m_menu;
}


QFileSystemModel * FileExplorer::model()
{
	return m_model;
}

QMenu * FileExplorer::menu()
{
	return m_menu;
}

QModelIndex FileExplorer::clickedFile()
{
	return m_clickedFile;
}


void FileExplorer::setFilter(const QDir::Filters & filters)
{
	m_model->setFilter(filters);
}


void FileExplorer::callSetRoot(const QModelIndex & index)
{
	QString rootPath = m_model->fileInfo(index).absoluteFilePath();
	this->setRoot(rootPath);
}

void FileExplorer::setClickedFile(const QModelIndex & index)
{
	m_clickedFile = index;
}

void FileExplorer::showContextMenu(const QPoint & point)
{
	this->setClickedFile(this->indexAt(point));
	m_menu->exec(this->mapToGlobal(point));
}


void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}