
#include "filewidget.h"

#include <QDialogButtonBox>

FileWidget::FileWidget(
    QWidget * parent
,   Qt::WindowFlags f)
:   QFileDialog(parent, f)
{
    initialize();
}

FileWidget::FileWidget(
    QWidget *parent
,   const QString & caption
,   const QString & directory
,   const QString & filter)
:   QFileDialog(parent, caption, directory, filter)
{
    initialize();
}

void FileWidget::initialize()
{
    this->setSizeGripEnabled(false);
    this->setModal(false);
    this->setWindowFlags(Qt::Widget);

    // hack: removing button box of filedialog
    QDialogButtonBox * buttonBox = this->findChild<QDialogButtonBox*>("buttonBox");
    if(buttonBox)
    {
        buttonBox->hide();
    }
}

FileWidget::~FileWidget()
{
    //
}

void FileWidget::done(int result)
{
    // nop
}

void FileWidget::accept()
{
    // nop
}
