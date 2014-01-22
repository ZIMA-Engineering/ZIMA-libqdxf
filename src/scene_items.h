/*
  ZIMA-libqdxf
  http://www.zima-engineering.cz/projekty/ZIMA-libqdxf

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCENE_ITEMS_H
#define SCENE_ITEMS_H

#include <QGraphicsItem>
#include "../libdxfrw/src/libdxfrw.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsEllipseItem>


class SceneArc : public QAbstractGraphicsShapeItem
{
public:
    SceneArc(DRW_Arc a)
    {
        arc = a;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /* widget */)
    {
        painter->setPen(pen());
        if(arc.isccw == 0)
            qDebug() << "not ccw";

        //qDebug() << "painting arc";
        int startAngle = -arc.staangle*(180*16/M_PI);
        int spanAngle;

        if(arc.endangle > arc.staangle)
            spanAngle = (arc.endangle-arc.staangle)*(180*16/M_PI);
        else
            spanAngle = (2*M_PI-arc.staangle+arc.endangle)*(180*16/M_PI);

        if(spanAngle > 0)
            spanAngle *= -1;

        /*qDebug() << "startangle:" << startAngle/16;
        qDebug() << "spanangle:" << spanAngle/16;

        qDebug() << "dxf: startangle" << arc.staangle*(180/M_PI);
        qDebug() << "dxf: endangle" << arc.endangle*(180/M_PI);*/

        painter->drawArc( QRectF(arc.basePoint.x-arc.radious, arc.basePoint.y-arc.radious, arc.radious*2, arc.radious*2), startAngle, spanAngle);
    }

    QRectF boundingRect() const
    {
        return QRectF(arc.basePoint.x-arc.radious, arc.basePoint.y-arc.radious, arc.radious*2, arc.radious*2);
    }

private:
    DRW_Arc arc;
};

#endif // SCENE_ITEMS_H
