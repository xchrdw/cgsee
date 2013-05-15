
#pragma once

#include <memory>

#include <QDialog>
#include <QString>
#include <QSize>

#include <core/declspec.h>

#include "canvasexportconfig.h"

class QLineEdit;
class QIntValidator;
class QDoubleValidator;

class AbstractPainter;
class Canvas;
class QSettings;


class Ui_CanvasExporter;

class CGSEE_API CanvasExporter : public QDialog
{
    Q_OBJECT

public:
    static const bool save(
        Canvas & canvas
    ,   QWidget * parent = nullptr
    ,   const bool advanced = false);

protected:
    CanvasExporter(
        const QSize & size
    ,   QWidget * parent = nullptr);
    virtual ~CanvasExporter();

    const QString filePath();
    const QString incrementLastFilePath();

    void update();

    void addDummyPreset(const QString & text);
    void addCustomPreset(
        const QString & text
    ,   const QSize & size);
    void addConfigPreset(const QString & text);

    void restoreState(const QSettings & s);
    void saveState(QSettings & s);

protected slots:
    void on_widthLineEdit_textEdited(const QString & text);
    void on_heightLineEdit_textEdited(const QString & text);

    void on_widthUnitComboBox_currentIndexChanged(int index);
    void on_heightUnitComboBox_currentIndexChanged(int index);

    void on_resolutionLineEdit_textEdited(const QString & text);

    void on_alphaCheckBox_stateChanged(int state);
    void on_aspectCheckBox_stateChanged(int state);

    void on_presetsComboBox_currentIndexChanged(int index);

protected:
    QLineEdit * m_lastSizeComponentChanged;

    QIntValidator * m_intValidator;
    QDoubleValidator * m_doubleValidator;

    const std::unique_ptr<Ui_CanvasExporter> m_ui;

    CanvasExportConfig * m_config;
    QList<CanvasExportConfig *> m_customPresets;
};
