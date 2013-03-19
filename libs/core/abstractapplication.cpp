
#include "abstractapplication.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapplication.h>

#include <QApplication>
#include <QFileInfo>
#include <QStringList>

#include "meta.h"
#include "glformat.h"
#include "messagehandler.h"


AbstractApplication::AbstractApplication(
	int & argc,
	char ** argv)

:   QApplication(argc, argv)
{
	const QFileInfo fi(QCoreApplication::applicationFilePath());

	QApplication::setApplicationDisplayName(fi.baseName());

    QApplication::setApplicationName(META_PROJECT_NAME);
    QApplication::setApplicationVersion(META_VERSION);

    QApplication::setOrganizationName(META_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(META_AUTHOR_DOMAIN);

    qInstallMessageHandler(globalMessageHandler);

    qDebug("%s on Qt-%s\n", qPrintable(title()), qPrintable(qVersion()));
}

AbstractApplication::~AbstractApplication()
{
}

const QString AbstractApplication::title() const
{
    return QString("%1 - %2 %3")
        .arg(QApplication::applicationDisplayName())
		.arg(QApplication::applicationName())
		.arg(QApplication::applicationVersion());
}

const GLFormat AbstractApplication::format() const
{
    GLFormat format;

    format.setVersion(3, 2);
    // Version 3.2 + yields CoreProfile context by default.
    format.setProfile(GLFormat::CompatibilityProfile);

    format.setVSync(true);

    // NOTE: If profile is core, extensions availability will not 
    // be verified, since they are not listed by glew.

    const QStringList extensions = QStringList() 
        << "GL_EXT_abgr"; // extension with number 1 ;)

    format.setExtensions(extensions);

    return format;
}
