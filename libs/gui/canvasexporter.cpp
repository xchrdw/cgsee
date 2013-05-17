
#include <cassert>

#include <QImageReader>
#include <QValidator>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QDateTime>

#include "ui_canvasexporter.h"
#include "canvasexporter.h"
#include "canvasexportconfig.h"

#include "canvas.h"

#include <core/abstractpainter.h>

namespace 
{
    const QString SETTINGS_GROUP        ("CanvasExport");
    
    const QString SETTINGS_GEOMETRY     ("Geometry");
    const QString SETTINGS_LASTFILEPATH ("LastSavePath");
    const QString SETTINGS_LASTPRESET   ("LastPreset");
    const QString SETTINGS_CUSTOMSIZE   ("CustomSize");
    const QString SETTINGS_CUSTOMPPI    ("CustomPixelPerInch");
    const QString SETTINGS_CUSTOMUNIT   ("CustomUnit");
    const QString SETTINGS_CUSTOMALPHA  ("CustomAlpha");
    const QString SETTINGS_CUSTOMASPECT ("CustomAspect");
}


CanvasExporter::CanvasExporter(
    const QSize & size
,   QWidget * parent)
:   QDialog(parent)
,   m_lastSizeComponentChanged(nullptr)
,   m_intValidator(new QIntValidator)
,   m_doubleValidator(new QDoubleValidator)
,   m_ui(new Ui_CanvasExporter)
,   m_config(new CanvasExportConfig)
{
    m_ui->setupUi(this);

    // add current res and last setting

    addDummyPreset (QObject::tr("Custom Configuration"));
    addCustomPreset(QObject::tr("Canvas    "), size * 1);
    addCustomPreset(QObject::tr("Canvas 2x "), size * 2);
    addCustomPreset(QObject::tr("Canvas 4x "), size * 4);
    addCustomPreset(QObject::tr("Canvas 8x "), size * 8);

    // add config presets 

    const QStringList & presetIDs = CanvasExportConfig::presetsIdentifier();
    for(const QString & identifier : presetIDs)
        addConfigPreset(identifier);

    // init units 

    m_ui->widthUnitComboBox-> addItems(CanvasExportConfig::unitNames());
    m_ui->heightUnitComboBox->addItems(CanvasExportConfig::unitNames());
    m_ui->resolutionUnitComboBox->addItem(CanvasExportConfig::ppiUnitIdentifier());

    // restore 

    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s);
    s.endGroup();

    // done ;)

    update();
}

CanvasExporter::~CanvasExporter()
{
    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    saveState(s);
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

    if(advanced && !ie.exec())
        return false;

    QString filePath;
    if(advanced)
    {
        filePath = ie.filePath();
        if(filePath.isEmpty())
            return false;
    }
    else
        filePath = ie.incrementLastFilePath();

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

void CanvasExporter::addDummyPreset(const QString & text)
{
    QAbstractItemModel * model = m_ui->presetsComboBox->model();
    m_ui->presetsComboBox->addItem(text);

    QModelIndex index = model->index(model->rowCount() - 1, 0);
    model->setData(index, QVariant(0), Qt::UserRole - 1);
}

void CanvasExporter::addCustomPreset(
    const QString & text
,   const QSize & size)
{
    m_customPresets << new CanvasExportConfig(size.width(), size.height(), 72, CanvasExportConfig::Pixels);
    m_ui->presetsComboBox->addItem(
        QString("%1%2 x %3 px").arg(text).arg(size.width(), 4).arg(size.height(), 4),
        QVariant(m_customPresets.size() - 1));
}

void CanvasExporter::addConfigPreset(const QString & text)
{
    QAbstractItemModel * model = m_ui->presetsComboBox->model();

    m_ui->presetsComboBox->addItem(text);

    const bool disable = text.startsWith("- ");

    QModelIndex index = model->index(model->rowCount() - 1, 0);
    model->setData(index, QVariant(0), Qt::UserRole + (disable ? - 1 : 1));
}

void CanvasExporter::restoreState(const QSettings & s)
{
    // select last preset/config
    const int lastPreset(s.value(SETTINGS_LASTPRESET, 1).toInt());
    if(0 == lastPreset)
    {
        if(s.contains(SETTINGS_CUSTOMUNIT))
            m_config->setUnit(static_cast<CanvasExportConfig::e_Unit>(
                s.value(SETTINGS_CUSTOMUNIT).toInt()));

        if(s.contains(SETTINGS_CUSTOMPPI))
                m_config->setPixelPerInch(s.value(SETTINGS_CUSTOMPPI).toDouble());

        if(s.contains(SETTINGS_CUSTOMSIZE))
        {
            const QSizeF customSize(s.value(SETTINGS_CUSTOMSIZE).toSizeF());
            m_config->setWidth(customSize.width());
            m_config->setHeight(customSize.height());
        }
        if(s.contains(SETTINGS_CUSTOMALPHA))
            m_config->setAlpha(s.value(SETTINGS_CUSTOMALPHA).toBool());
        if(s.contains(SETTINGS_CUSTOMASPECT))
            m_config->setAspect(s.value(SETTINGS_CUSTOMASPECT).toBool());
    }
    else if(lastPreset < m_ui->presetsComboBox->count())
        m_ui->presetsComboBox->setCurrentIndex(lastPreset);
}

void CanvasExporter::saveState(QSettings & s)
{
    s.setValue(SETTINGS_LASTPRESET, m_ui->presetsComboBox->currentIndex());

    if(m_ui->presetsComboBox->currentIndex())
        return;

    s.setValue(SETTINGS_CUSTOMUNIT, m_config->unit());
    s.setValue(SETTINGS_CUSTOMPPI, m_config->ppi());
    s.setValue(SETTINGS_CUSTOMSIZE, QSizeF(m_config->width(), m_config->height()));
    s.setValue(SETTINGS_CUSTOMALPHA, m_config->alpha());
    s.setValue(SETTINGS_CUSTOMASPECT, m_config->aspect());
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

    QString filePath = s.value(SETTINGS_LASTFILEPATH).toString();

    filePath = dialog.getSaveFileName(this, QObject::tr("Save as Image"), filePath, filter); 

    if(!filePath.isEmpty())
        s.setValue(SETTINGS_LASTFILEPATH, filePath);

    s.endGroup();

    return filePath;
}

const QString CanvasExporter::incrementLastFilePath()
{
    QSettings s;
    s.beginGroup(SETTINGS_GROUP);
    QString filePath = s.value(SETTINGS_LASTFILEPATH).toString();
    s.endGroup();

    if(filePath.isEmpty())
        return CanvasExporter::filePath();

    const int doti = filePath.lastIndexOf(".");
    
    filePath = filePath.left(doti)
        + "_" + QDateTime::currentDateTime().toString("hhmmsszzz")
        + filePath.right(filePath.size() - doti);

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