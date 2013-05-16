
#include "fileExplorer.h"


static const QStringList nameFilters = (QStringList() << "*.obj" << "*.txt");

FileExplorer::FileExplorer(
	QObject * parent)

:	m_navigator(nullptr)
,	m_model(nullptr)
,	m_menu(nullptr)
,	m_clickedFile()
{
	m_model = new QFileSystemModel;
	this->setModel(m_model);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	QObject::connect(
		this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));

	setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setRoot(QDir::currentPath());

	m_model->setNameFilters(nameFilters);
	m_model->setNameFilterDisables(false);

	m_menu = new QMenu;
	m_menu->addAction(QString("Open"), this, SLOT(triggeredLoadFile(const bool)));
};

void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
}

void FileExplorer::setFilter(QDir::Filters filters)
{
	m_model->setFilter(filters);
}

#include <iostream> // just for debug output
void FileExplorer::loadFile(const QModelIndex & index)
{
	QString filePath = m_model->fileInfo(index).absoluteFilePath();
	std::cout << filePath.toStdString() << std::endl;

	// QString filePath2 = m_model->fileInfo(this->selectionModel()->selectedIndexes().first()).absolutePath();
	// std::cout << filePath2.toStdString() << std::endl;
}

void FileExplorer::callSetRoot(const QModelIndex & index)
{
	QString rootPath = m_model->fileInfo(index).absoluteFilePath();
	this->setRoot(rootPath);
}

void FileExplorer::showContextMenu(const QPoint & point)
{
	m_clickedFile = this->indexAt(point);
	
	m_menu->exec(this->mapToGlobal(point));
}

void FileExplorer::triggeredLoadFile(const bool & triggered)
{
	this->loadFile(m_clickedFile);
}

FileExplorer::~FileExplorer()
{
	delete m_model;
	delete m_menu;
}

void FileExplorer::setNavigator(FileNavigator * fileNavigator)
{
	m_navigator = fileNavigator;
}

FileNavigator * FileExplorer::navigator()
{
	return m_navigator;
}