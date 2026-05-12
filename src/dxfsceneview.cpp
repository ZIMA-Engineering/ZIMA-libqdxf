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

#include "dxfsceneview.h"

DXFSceneView::DXFSceneView(QWidget *parent) :
    QGraphicsView(parent)
{
    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(NoDrag);
    setCursor(Qt::ArrowCursor);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(1, -1);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void DXFSceneView::fitAll()
{
    if (!scene())
        return;

    const QRectF bounds = scene()->itemsBoundingRect();
    if (bounds.isEmpty())
        return;

    fitInView(bounds, Qt::KeepAspectRatio);
}

void DXFSceneView::wheelEvent(QWheelEvent* event)
{

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->angleDelta().y() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

}
