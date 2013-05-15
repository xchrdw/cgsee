
#include "fileExplorer.h"


FileExplorer::FileExplorer(
	QObject * parent)

:	m_navigator(nullptr)
,	m_model(nullptr)
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	QObject::connect(
		this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(userContextMenuRequested(const QPoint &)));

	setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setRoot(QDir::currentPath());
};

void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileExplorer::setFilter(QDir::Filters filters)
{
	m_model->setFilter(filters);
}

void FileExplorer::callSetRoot(const QModelIndex & index)
{
	QString rootPath = m_model->fileInfo(index).absoluteFilePath();
	this->setRoot(rootPath);
}

void FileExplorer::userContextMenuRequested(const QPoint & point)
{
	QMenu *menu = new QMenu;
	menu->addAction(QString("Test Item"), this, SLOT(test_slot()));
	menu->exec(this->mapToGlobal(point));
}

FileExplorer::~FileExplorer()
{
	delete m_model;
}

void FileExplorer::setNavigator(FileNavigator * fileNavigator)
{
	m_navigator = fileNavigator;
}

FileNavigator * FileExplorer::navigator()
{
	return m_navigator;
}