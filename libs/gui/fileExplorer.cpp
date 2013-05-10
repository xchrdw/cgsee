
#include "fileExplorer.h"


FileExplorer::FileExplorer(
	QObject * parent)

:	m_navigator(nullptr)
,	m_model(nullptr)
,	m_dock(nullptr)
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);

	setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setRoot(QDir::currentPath());

	m_dock = new QDockWidget(tr("Navigator"));
	m_dock->setWidget(this);
	m_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
};

void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileExplorer::setFilter(QDir::Filters filters)
{
	m_model->setFilter(filters);
}

FileExplorer::~FileExplorer()
{
	delete m_model;
	// delete m_dock;
}

void FileExplorer::setNavigator(FileNavigator * fileNavigator)
{
	m_navigator = fileNavigator;
}

FileNavigator * FileExplorer::navigator()
{
	return m_navigator;
}

QDockWidget * FileExplorer::dock()
{
	return m_dock;
}