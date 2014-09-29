
#include <core/abstractapplication.h>

#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>

#include <cgsee-version.h>

#include <core/glformat.h>
#include <core/messagehandler.h>


AbstractApplication::AbstractApplication(
    int & argc,
    char ** argv)

:   QApplication(argc, argv)
{
    const QFileInfo fi(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());

    QApplication::setApplicationName(CGSEE_PROJECT_NAME);
    QApplication::setApplicationVersion(CGSEE_VERSION);

    QApplication::setOrganizationName(CGSEE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(CGSEE_AUTHOR_DOMAIN);

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
    format.setProfile(GLFormat::CoreProfile);

    format.setVSync(false);

    // NOTE: If profile is core, extensions availability will not 
    // be verified, since they are not listed by gl ew.

    const QStringList extensions = QStringList() 
        << "GL_EXT_abgr" // extension with number 1 ;)
        << "AMD_interleaved_elements";

    format.setExtensions(extensions);

    return format;
}
