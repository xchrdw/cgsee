
#pragma once

#include <QFileDialog>

#include <core/declspec.h>

class QFileDialog;

// see http://www.qtforum.de/forum/viewtopic.php?t=13714

class CGSEE_API FileWidget : public QFileDialog
{
    Q_OBJECT
public:
    FileWidget(QWidget *parent, Qt::WindowFlags f);

    explicit FileWidget(QWidget *parent = 0,
                         const QString &caption = QString(),
                         const QString &directory = QString(),
                         const QString &filter = QString());

    virtual ~FileWidget();

    virtual void initialize();

protected:
    void done(int result);
    void accept();
};
