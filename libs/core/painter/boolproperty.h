
#pragma once

#include <core/declspec.h>
#include "abstractpainterproperty.h"

class CGSEE_API BoolProperty : public AbstractPainterProperty
{
public:
    BoolProperty();
    explicit BoolProperty(QString name);
    explicit BoolProperty(bool enabled);
    BoolProperty(QString name, bool enabled);
    virtual ~BoolProperty();

    bool enabled() const;
    void setEnabled(bool enabled);
private:
    bool m_enabled;
};