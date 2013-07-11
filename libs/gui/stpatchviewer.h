#include "core/stpatch.h"
#include "declspec.h"

#include <QWidget>

class CGSEE_GUI_API StPatchViewer : public QWidget
{
public:
    StPatchViewer(StPatch const& observable, QWidget *parent = nullptr);

protected:
    virtual void paintEvent( QPaintEvent * eve );

protected:
    StPatch m_Patch;
};