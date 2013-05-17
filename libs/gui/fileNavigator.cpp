
#include "fileNavigator.h"


FileNavigator::FileNavigator(
	QObject * parent)

:	m_explorer(nullptr)
,	m_model(nullptr)
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);

	setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	setRoot(QDir::currentPath());

	this->setColumnHidden(1, true);
	this->setColumnHidden(2, true);
	this->setColumnHidden(3, true);
};

#include <iostream>
void FileNavigator::setRoot(QString rootPath)
{
	// this->setRootIndex(m_model->setRootPath(rootPath));
	m_model->setRootPath(rootPath);
	std::cout << rootPath.toStdString() << std::endl;
}

void FileNavigator::setFilter(QDir::Filters filters)
{
	m_model->setFilter(filters);
}

FileNavigator::~FileNavigator()
{
	delete m_model;
}

void FileNavigator::setExplorer(FileExplorer * fileExplorer)
{
	m_explorer = fileExplorer;
}

FileExplorer * FileNavigator::explorer()
{
	return m_explorer;
}