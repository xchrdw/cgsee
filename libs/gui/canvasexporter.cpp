
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


CanvasExporter::CanvasExporter(
    const QSize & size
,   QWidget * parent)
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

    // add current res and last setting
    
    QAbstractItemModel * model = m_ui->presetsComboBox->model();
    const QVariant v(0);

    m_ui->presetsComboBox->addItem("Custom Configuration");
    QModelIndex index = model->index(model->rowCount() - 1, 0);
    model->setData(index, v, Qt::UserRole - 1);

    m_customPresets << new CanvasExportConfig(size.width(), size.height(), 72, CanvasExportConfig::Pixels);
    m_ui->presetsComboBox->addItem(
        QString("Canvas    %1 x %2 px").arg(size.width(), 4).arg(size.height(), 4),
        QVariant(m_customPresets.size() - 1));

    m_customPresets << new CanvasExportConfig(size.width() * 2, size.height() * 2, 72, CanvasExportConfig::Pixels);
    m_ui->presetsComboBox->addItem(
        QString("Canvas 2x %1 x %2 px").arg(size.width() * 2, 4).arg(size.height() * 2, 4),
        QVariant(m_customPresets.size() - 1));

    m_customPresets << new CanvasExportConfig(size.width() * 4, size.height() * 4, 72, CanvasExportConfig::Pixels);
    m_ui->presetsComboBox->addItem(
        QString("Canvas 4x %1 x %2 px").arg(size.width() * 4, 4).arg(size.height() * 4, 4),
        QVariant(m_customPresets.size() - 1));

    m_customPresets << new CanvasExportConfig(size.width() * 8, size.height() * 8, 72, CanvasExportConfig::Pixels);
    m_ui->presetsComboBox->addItem(
        QString("Canvas 8x %1 x %2 px").arg(size.width() * 8, 4).arg(size.height() * 8, 4),
        QVariant(m_customPresets.size() - 1));

    // add config presets 

    const QStringList & presetIDs = CanvasExportConfig::presetsIdentifier();
    for(const QString & identifier : presetIDs)
    {
        m_ui->presetsComboBox->addItem(identifier);

        const bool disable = identifier.startsWith("- ");

        QModelIndex index = model->index(model->rowCount() - 1, 0);
        model->setData(index, v, Qt::UserRole + (disable ? - 1 : 1));
    }

    // configure "set this config as default" button

    QPushButton * defaultButton = m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    assert(defaultButton);
    defaultButton->setText(QObject::tr("&Default"));
    defaultButton->setCheckable(true);

    // TODO: set button checked if default is active...

    // init units 

    m_ui->widthUnitComboBox->blockSignals(true);
    m_ui->widthUnitComboBox-> addItems(CanvasExportConfig::unitNames());
    m_ui->widthUnitComboBox->blockSignals(false);

    m_ui->heightUnitComboBox->blockSignals(true);
    m_ui->heightUnitComboBox->addItems(CanvasExportConfig::unitNames());
    m_ui->heightUnitComboBox->blockSignals(false);
    
    m_ui->resolutionUnitComboBox->blockSignals(true);
    m_ui->resolutionUnitComboBox->addItem(CanvasExportConfig::ppiUnitIdentifier());
    m_ui->resolutionUnitComboBox->blockSignals(false);

    m_ui->resolutionLineEdit->setValidator(m_doubleValidator);


    update();

    // TODO: restore with QSettings
    // - all other settings (alpha, size, aspect, units displayed in ...)
}

CanvasExporter::~CanvasExporter()
{
    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.endGroup();

    if(!m_customPresets.contains(m_config))
        delete m_config;

    qDeleteAll(m_customPresets);
    m_customPresets.clear();

    delete m_intValidator;
    delete m_doubleValidator;
}

const bool CanvasExporter::save(
    Canvas & canvas
,   QWidget * parent
,   const bool advanced)
{
    CanvasExporter ie(canvas.size(), parent);

    if(!ie.exec())
        return false;

    const QString filePath(ie.filePath());
    if(filePath.isEmpty())
        return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    QImage image(canvas.capture(ie.m_config->size(), ie.m_config->aspect(), ie.m_config->alpha()));
    const bool result = image.save(filePath);

    QApplication::restoreOverrideCursor();

    if(!result)
        qWarning("Image export to \"%s\" failed", qPrintable(filePath));

    return result;
}

void CanvasExporter::update()
{
    if(m_config->modified() && m_ui->presetsComboBox->currentIndex())
        m_ui->presetsComboBox->setCurrentIndex(0);

    const int unit = static_cast<int>(m_config->unit());

    m_ui->widthUnitComboBox->blockSignals(true);
    m_ui->widthUnitComboBox->setCurrentIndex(unit);
    m_ui->widthUnitComboBox->blockSignals(false);

    m_ui->heightUnitComboBox->blockSignals(true);
    m_ui->heightUnitComboBox->setCurrentIndex(unit);
    m_ui->heightUnitComboBox->blockSignals(false);

    bool pixels = CanvasExportConfig::Pixels == m_config->unit();
    const int prec = pixels ?  0  :  4 ;
    const int f    = pixels ? 'f' : 'g';

    QValidator * validator(nullptr);
    if(pixels)
        validator = m_intValidator;
    else
        validator = m_doubleValidator;

    if(m_ui->widthLineEdit->validator() != validator)
        m_ui->widthLineEdit->setValidator(validator);
    if(m_ui->widthLineEdit != m_lastSizeComponentChanged)
        m_ui->widthLineEdit->setText(QString::number(m_config->width(), f, prec));

    if(m_ui->heightLineEdit->validator() != validator)
        m_ui->heightLineEdit->setValidator(validator);
    if(m_ui->heightLineEdit != m_lastSizeComponentChanged)
        m_ui->heightLineEdit->setText(QString::number(m_config->height(), f, prec));

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

void CanvasExporter::on_presetsComboBox_currentIndexChanged(int index)
{
    if(index == 0)  // in this case the custom configuration is active
        return;

    if(index < m_customPresets.size() + 1)  // these are custom presets
    {
        if(!m_customPresets.contains(m_config))
            delete m_config;

        m_config = m_customPresets[index - 1];
        update();

        return;
    }

    // else, common presets from config can be retrieved

    QAbstractItemModel * model = m_ui->presetsComboBox->model();
    if(!model->data(model->index(index, 0), Qt::UserRole + 1).isValid())
        return;

    QString identifier(m_ui->presetsComboBox->itemText(index));
    CanvasExportConfig * config(CanvasExportConfig::preset(identifier));
    if(!config)
        return;

    if(!m_customPresets.contains(m_config))
        delete m_config;

    m_config = config;

    update();
}