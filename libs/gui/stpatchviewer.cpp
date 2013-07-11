#include "stpatchviewer.h"
#include <QtGui>
#include <glm\core\type.hpp>

StPatchViewer::StPatchViewer(StPatch const& observable, QWidget* parent):
    QWidget(parent)
,   m_Patch(observable)
{
    this->setMinimumSize(250, 250);
}

void StPatchViewer::paintEvent( QPaintEvent * eve )
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);

    glm::vec2 minc, maxc;
    m_Patch.calculateAabb(minc, maxc);

    //painter.translate(clientWidth()/2, height()/2);
    painter.scale((width() ) / (maxc - minc).x, (height() ) / (maxc - minc).y);
    painter.translate(-minc.x , -minc.y );

    QPen oldPen = painter.pen();
    oldPen.setCosmetic(true);
    painter.setPen(oldPen);

    for (int i = 0; i < m_Patch.countLines(); ++i)
    {
        glm::vec4 line = m_Patch.getLine(i);
        painter.drawLine(QPointF(line.x, line.y), QPointF(line.z, line.w));
    }

    QList <glm::vec2> outerHull;
    m_Patch.calculateConvexHull(outerHull);
    oldPen.setColor(Qt::red);
    oldPen.setWidth(2);
    painter.setPen(oldPen);

    for (int i = 1; i < outerHull.size(); ++i)
    {
        painter.drawLine(QPointF(outerHull[i-1].x, outerHull[i-1].y), 
            QPointF(outerHull[i].x, outerHull[i].y));
    }
}