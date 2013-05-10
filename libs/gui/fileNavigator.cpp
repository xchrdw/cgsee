
#include "fileNavigator.h"


FileNavigator::FileNavigator(
	QObject * parent)

:	m_explorer(nullptr)
,	m_model(nullptr)
,	m_dock(nullptr)
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);

	setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	setRoot(QDir::currentPath());

	m_dock = new QDockWidget(tr("Navigator"));
	m_dock->setWidget(this);
	m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
};

void FileNavigator::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileNavigator::setFilter(QDir::Filters filters)
{
	m_model->setFilter(filters);
}

FileNavigator::~FileNavigator()
{
	delete m_model;
	// delete m_dock;
}

void FileNavigator::setExplorer(FileExplorer * fileExplorer)
{
	m_explorer = fileExplorer;
}

FileExplorer * FileNavigator::explorer()
{
	return m_explorer;
}

QDockWidget * FileNavigator::dock()
{
	return m_dock;
}