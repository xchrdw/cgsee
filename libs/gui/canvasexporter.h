
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
    CanvasExporter(QWidget * parent = nullptr);
    virtual ~CanvasExporter();

    void update();

protected slots:
    void on_widthLineEdit_textEdited(const QString & text);
    void on_heightLineEdit_textEdited(const QString & text);

    void on_widthUnitComboBox_currentIndexChanged(int index);
    void on_heightUnitComboBox_currentIndexChanged(int index);

    void on_resolutionLineEdit_textEdited(const QString & text);

protected:
    QLineEdit * m_lastSizeComponentChanged;

    QIntValidator * m_intValidator;
    QDoubleValidator * m_doubleValidator;

    const std::unique_ptr<Ui_CanvasExporter> m_ui;

    CanvasExportConfig * m_config;
    QSize m_canvasSize;
};
