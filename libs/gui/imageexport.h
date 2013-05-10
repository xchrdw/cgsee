
#pragma once

#include <memory>

#include <QDialog>

#include <core/declspec.h>

class AbstractPainter;


class Ui_ImageExport;

class CGSEE_API ImageExport : public QDialog
{
    Q_OBJECT

public:
    static const bool save(
        AbstractPainter & painter
    ,   QWidget * parent = nullptr);

public:
    ImageExport(QWidget * parent = nullptr);
    ImageExport(
        const QImage & image
    ,   QWidget * parent = nullptr);

    virtual ~ImageExport();

    const bool save() const;
    const bool save(const QImage & image) const;

    const bool aspect() const;
    const bool alpha() const;

    const QSize & size() const;

    const QString & filePath() const;

protected:
    void initialize();

    QWidget * m_parent;

    const std::unique_ptr<Ui_ImageExport> m_ui;

    mutable QString m_filePath;
    mutable QImage m_image;

    bool m_alpha;
    bool m_aspect;

    QSize m_size;
};
