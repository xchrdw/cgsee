
#include "fileExplorer.h"


FileExplorer::FileExplorer(
	QObject * parent)

:	m_navigator(nullptr)
,	m_model(nullptr)
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);

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