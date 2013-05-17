
#include "fileExplorer.h"


static const QStringList nameFilters = (QStringList() << "*.obj" << "*.txt");

FileExplorer::FileExplorer(
	QObject * parent)

:	m_navigator(nullptr)
,	m_model(new QFileSystemModel)
,	m_menu(new QMenu)
,	m_clickedFile()
{
	setModel(m_model);
	setContextMenuPolicy(Qt::CustomContextMenu);

	setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setRoot(QDir::homePath());

	m_model->setNameFilters(nameFilters);
	m_model->setNameFilterDisables(false);

	m_menu->addAction(QString("Open"), this, SLOT(triggeredLoadFile(const bool)));

	QObject::connect(
		this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));

	QObject::connect(
        this, SIGNAL(activated(const QModelIndex)),
        this, SLOT(loadFile(const QModelIndex)));
};

FileExplorer::~FileExplorer()
{
	delete m_model;
	delete m_menu;
}


FileNavigator * FileExplorer::navigator()
{
	return m_navigator;
}


void FileExplorer::setNavigator(FileNavigator * fileNavigator)
{
	m_navigator = fileNavigator;
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

void FileExplorer::triggeredLoadFile(const bool & triggered)
{
	this->loadFile(m_clickedFile);
}

void FileExplorer::showContextMenu(const QPoint & point)
{
	this->setClickedFile(this->indexAt(point));
	m_menu->exec(this->mapToGlobal(point));
}

#include <iostream> // just for debug output
void FileExplorer::loadFile(const QModelIndex & index)
{
	QString filePath = m_model->fileInfo(index).absoluteFilePath();
	std::cout << filePath.toStdString() << std::endl;
}


void FileExplorer::setRoot(QString rootPath)
{
	this->setRootIndex(m_model->setRootPath(rootPath));
	// m_model->setRootPath(rootPath);
}