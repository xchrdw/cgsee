
#include <QFileSystemModel>

#include "fileNavigator.h"


FileNavigator::FileNavigator(
	QObject * parent)

:	m_model(new QFileSystemModel)
{
	this->setModel(m_model);

	setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	setRoot(QDir::homePath());

	this->setColumnHidden(1, true);
	this->setColumnHidden(2, true);
	this->setColumnHidden(3, true);
};

void FileNavigator::setRoot(const QString & rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileNavigator::setFilter(const QDir::Filters & filters)
{
	m_model->setFilter(filters);
}

FileNavigator::~FileNavigator()
{
	delete m_model;
}