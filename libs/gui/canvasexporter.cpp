
#include <QImageReader>
#include <QValidator>
#include <QDoubleValidator>
#include <QIntValidator>

#include "ui_canvasexporter.h"
#include "canvasexporter.h"
#include "canvasexportconfig.h"

#include "canvas.h"

#include <core/abstractpainter.h>


CanvasExporter::CanvasExporter(QWidget * parent)
:   QDialog(parent)
,   m_ui(new Ui_CanvasExporter)
,   m_config(new CanvasExportConfig)
,   m_lastSizeComponentChanged(nullptr)
,   m_intValidator(new QIntValidator)
,   m_doubleValidator(new QDoubleValidator)
{
    m_ui->setupUi(this);

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

    //QApplication::setOverrideCursor(Qt::WaitCursor);
    //QApplication::processEvents();

    //QImage image(painter.capture(ie.size(), ie.aspect(), ie.alpha()));
    //const bool result = ie.save(image);

    //QApplication::restoreOverrideCursor();

    //if(!result)
    //    qWarning("Image export to \"%s\" failed", qPrintable(ie.filePath()));

    return false;
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

    m_ui->resolutionLineEdit->blockSignals(true);
    m_ui->resolutionLineEdit->setText(QString::number(m_config->ppi(), 'g', 4));
    m_ui->resolutionLineEdit->blockSignals(false);

    m_ui->sizeInfoLabel->setText(m_config->sizeString());
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


//
//const bool CanvasExporter::show()        // TODO: do this on OK event... 
//{
//    //m_filewidget = new FileWidget(this, "FileDialog");
//
//    //static QString filter;
//    //if(filter.isEmpty())
//    //{
//    //    const QList<QByteArray> formats = QImageReader::supportedImageFormats();
//    //    QStringList extensions;
//
//    //    foreach(const QByteArray & format, formats)
//    //        extensions << QString("*." + QString::fromLocal8Bit(format).toLower());
//
//    //    filter = QObject::tr("Images (%1)\nAll Files (*.*)").arg(extensions.join("; "));
//    //}
//
//    //m_filewidget->setNameFilter(filter);
//
//
//
//
//    //if (this->exec() == QDialog::Accepted) {
//    //    m_filePath = m_filewidget->selectedFiles().value(0);
//    //    return !m_filePath.isEmpty();
//    //}
//
//    return false;
//}
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