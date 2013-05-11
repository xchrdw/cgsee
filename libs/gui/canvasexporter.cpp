
#include <cassert>

#include <QImageReader>
#include <QValidator>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>

#include "ui_canvasexporter.h"
#include "canvasexporter.h"
#include "canvasexportconfig.h"

#include "canvas.h"

#include <core/abstractpainter.h>

namespace 
{
    const QString SETTINGS_GROUP        ("CanvasExport");
    const QString SETTINGS_GEOMETRY     ("Geometry");
    const QString SETTINGS_LASTFILEPATH ("LastSaveImagePath");
}


CanvasExporter::CanvasExporter(QWidget * parent)
:   QDialog(parent)
,   m_ui(new Ui_CanvasExporter)
,   m_config(new CanvasExportConfig)
,   m_lastSizeComponentChanged(nullptr)
,   m_intValidator(new QIntValidator)
,   m_doubleValidator(new QDoubleValidator)
{
    m_ui->setupUi(this);

    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    s.endGroup();


    QPushButton * defaultButton = m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    assert(defaultButton);
    defaultButton->setText(QObject::tr("&Default"));
    defaultButton->setCheckable(true);

    // TODO: set button checked if default is active...

    m_ui->widthUnitComboBox->blockSignals(true);
    m_ui->widthUnitComboBox-> addItems(CanvasExportConfig::unitNames());
    m_ui->widthUnitComboBox->blockSignals(false);

    m_ui->heightUnitComboBox->blockSignals(true);
    m_ui->heightUnitComboBox->addItems(CanvasExportConfig::unitNames());
    m_ui->heightUnitComboBox->blockSignals(false);
    
    m_ui->resolutionUnitComboBox->blockSignals(true);
    m_ui->resolutionUnitComboBox->addItem(CanvasExportConfig::ppiUnitName());
    m_ui->resolutionUnitComboBox->blockSignals(false);

    m_ui->resolutionLineEdit->setValidator(m_doubleValidator);

    update();

    // TODO: restore with QSettings
    // - last file path
    // - all other settings (alpha, size, aspect, units displayed in ...)
}

CanvasExporter::~CanvasExporter()
{
    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.endGroup();


    delete m_config;

    delete m_intValidator;
    delete m_doubleValidator;
}

const bool CanvasExporter::save(
    Canvas & canvas
,   QWidget * parent
,   const bool advanced)
{
    CanvasExporter ie(parent);

    ie.m_canvasSize = canvas.size();

    if(!ie.exec())
        return false;

    const QString filePath(ie.filePath());
    if(filePath.isEmpty())
        return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    AbstractPainter * painter(canvas.painter());
    if(!painter)
    {
        qCritical("The is no painter available for capturing the canvas.");
        return false;
    }

    QImage image(painter->capture(ie.m_config->size(), ie.m_config->aspect(), ie.m_config->alpha()));
    const bool result = image.save(filePath);

    QApplication::restoreOverrideCursor();

    if(!result)
        qWarning("Image export to \"%s\" failed", qPrintable(filePath));

    return result;
}

void CanvasExporter::update()
{
    const int index = static_cast<int>(m_config->unit());

    m_ui->widthUnitComboBox->blockSignals(true);
    m_ui->widthUnitComboBox->setCurrentIndex(index);
    m_ui->widthUnitComboBox->blockSignals(false);

    m_ui->heightUnitComboBox->blockSignals(true);
    m_ui->heightUnitComboBox->setCurrentIndex(index);
    m_ui->heightUnitComboBox->blockSignals(false);

    QValidator * validator(nullptr);
    if(CanvasExportConfig::Pixels == m_config->unit())
        validator = m_intValidator;
    else
        validator = m_doubleValidator;

    if(m_ui->widthLineEdit->validator() != validator)
        m_ui->widthLineEdit->setValidator(validator);
    if(m_ui->widthLineEdit != m_lastSizeComponentChanged)
        m_ui->widthLineEdit->setText(QString::number(m_config->width(), 'g', 4));

    if(m_ui->heightLineEdit->validator() != validator)
        m_ui->heightLineEdit->setValidator(validator);
    if(m_ui->heightLineEdit != m_lastSizeComponentChanged)
        m_ui->heightLineEdit->setText(QString::number(m_config->height(), 'g', 4));

    m_ui->resolutionLineEdit->setText(QString::number(m_config->ppi(), 'g', 4));

    m_ui->sizeInfoLabel->setText(m_config->sizeString());

    m_ui->aspectCheckBox->setChecked(m_config->aspect());
    m_ui->alphaCheckBox->setChecked(m_config->alpha());
}

void CanvasExporter::on_widthLineEdit_textEdited(const QString & text)
{
    bool succeeded(false);
    const qreal width = text.toDouble(&succeeded);

    if(succeeded)
        m_config->setWidth(width);
    m_lastSizeComponentChanged = m_ui->widthLineEdit;

    update();
}

void CanvasExporter::on_heightLineEdit_textEdited(const QString & text)
{
    bool succeeded(false);
    const qreal height = text.toDouble(&succeeded);
    
    if(succeeded)
        m_config->setHeight(height);
    m_lastSizeComponentChanged = m_ui->heightLineEdit;

    update();
}

void CanvasExporter::on_widthUnitComboBox_currentIndexChanged(int index)
{
    const CanvasExportConfig::e_Unit unit = 
        static_cast<CanvasExportConfig::e_Unit>(m_ui->widthUnitComboBox->currentIndex());

    m_config->setUnit(unit);

    if(m_ui->heightLineEdit == m_lastSizeComponentChanged)
        m_lastSizeComponentChanged = nullptr;

    update();
}
 
void CanvasExporter::on_heightUnitComboBox_currentIndexChanged(int index)
{
    const CanvasExportConfig::e_Unit unit = 
        static_cast<CanvasExportConfig::e_Unit>(m_ui->heightUnitComboBox->currentIndex());

    m_config->setUnit(unit);

    if(m_ui->widthLineEdit == m_lastSizeComponentChanged)
        m_lastSizeComponentChanged = nullptr;

    update();
}

void CanvasExporter::on_resolutionLineEdit_textEdited(const QString & text)
{
    bool succeeded(false);
    const qreal ppi = text.toDouble(&succeeded);
    
    if(succeeded)
        m_config->setPixelPerInch(ppi);
    m_lastSizeComponentChanged = nullptr;

    update();
}

void CanvasExporter::on_alphaCheckBox_stateChanged(int state)
{
    m_config->setAlpha(state == Qt::Checked);
    update();
}

void CanvasExporter::on_aspectCheckBox_stateChanged(int state)
{
    m_config->setAspect(state == Qt::Checked);
    update();
}

const QString CanvasExporter::filePath()
{
    // TODO: advanced option should generate a name on last position

    QFileDialog dialog;
    
    static QString filter;
    if(filter.isEmpty())
    {
        const QList<QByteArray> formats = QImageReader::supportedImageFormats();
        QStringList extensions;

        foreach(const QByteArray & format, formats)
            extensions << QString("*." + QString::fromLocal8Bit(format).toLower());

        filter = QObject::tr("Images (%1)\nAll Files (*.*)").arg(extensions.join(";"));
    }

    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    const QString laseFilePath(s.value(SETTINGS_LASTFILEPATH).toString());
    s.endGroup();

    QString filePath = dialog.getSaveFileName(this, QObject::tr("Save as Image"), laseFilePath, filter); 

    return filePath;
}
//
//const bool CanvasExporter::save(
//    const QImage & image) const
//{
//    m_image = image;
//    return save();
//}
//
//const bool CanvasExporter::save() const
//{
//    if(m_filePath.isEmpty())
//        return false;
//
//    if(m_image.isNull())
//    {
//        qWarning("No image for export.");
//        return false;
//    }
//
//    return m_image.save(m_filePath);
//}
//
//const bool CanvasExporter::aspect() const
//{
//    return m_aspect;
//}
//
//const bool CanvasExporter::alpha() const
//{
//    return m_alpha;
//}
//
//const QSize & CanvasExporter::size() const
//{
//    return m_size;
//}
//
//const QString & CanvasExporter::filePath() const
//{
//    return m_filePath;
//}
//}