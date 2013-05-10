
#pragma once

#include <memory>

#include <QDialog>

#include <core/declspec.h>

class Ui_ImageExport;
class QFileDialog;

class CGSEE_API ImageExport : public QDialog
{
    Q_OBJECT
public:
    ImageExport(QWidget * parent = nullptr);
    ImageExport(
        const QImage & image
    ,   QWidget * parent = nullptr);

    virtual ~ImageExport();

    const bool show();

    const bool save() const;
    const bool save(const QImage & image) const;

protected:
    void initialize();

    QWidget * m_parent;

    const std::unique_ptr<Ui_ImageExport> m_ui;
    QFileDialog * m_filedialog;

    mutable QString m_filePath;
    mutable QImage m_image;
};
